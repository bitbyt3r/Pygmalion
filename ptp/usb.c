#include <pthread.h>
#include "libusb.h"
#include "usb.h"
#include "util.h"
#include "camera.h"
#include <stdbool.h>
#include <stdio.h>

bool done = true;
libusb_context *ctx = NULL;
pthread_t recv_thread_id;
static void (*camera_added)(libusb_device*, struct libusb_device_descriptor*) = NULL;
static void (*camera_removed)(libusb_device*, struct libusb_device_descriptor*) = NULL;
static bool (*camera_test)(struct libusb_device_descriptor*) = NULL;

static int LIBUSB_CALL hotplug_callback_attach(libusb_context *ctx, libusb_device *dev, libusb_hotplug_event event, void *user_data) {
    struct libusb_device_descriptor *desc;
    desc = (struct libusb_device_descriptor*)malloc(sizeof(struct libusb_device_descriptor));
    int err;
    err = libusb_get_device_descriptor(dev, desc);
    if (err != LIBUSB_SUCCESS) {
        printf("Failed to get device descriptor\n");
        return 0;
    }
    //printf("Device attached: %04x:%04x\n", desc->idVendor, desc->idProduct);
    if (camera_test != NULL) {
        if (!(*camera_test)(desc)) {
            return 0;
        }
    }

    if (camera_added != NULL) {
        (*camera_added)(dev, desc);
    }
    return 0;
}

static int LIBUSB_CALL hotplug_callback_detach(libusb_context *ctx, libusb_device *dev, libusb_hotplug_event event, void *user_data) {
    if (camera_removed != NULL) {
        struct libusb_device_descriptor *desc;
        desc = (struct libusb_device_descriptor*)malloc(sizeof(struct libusb_device_descriptor));
        int err;
        err = libusb_get_device_descriptor(dev, desc);
        if (err != LIBUSB_SUCCESS) {
            printf("Failed to get device descriptor\n");
            return 0;
        }
        (*camera_removed)(dev, desc);
    }
    return 0;
}

void *usb_recv_thread(void *vargp) {
    while (!done) {
        libusb_handle_events(ctx);
    }
    pthread_exit(0);
}

int ptp_usb_start(void *camera_added_cb, void *camera_removed_cb, void *camera_test_cb) {
    int err = 0;
    if (done) {
        // Register callbacks
        if (camera_added_cb != NULL) {
            camera_added = camera_added_cb;
        }
        if (camera_removed_cb != NULL) {
            camera_removed = camera_removed_cb;
        }
        if (camera_test_cb != NULL) {
            camera_test = camera_test_cb;
        }

        // Setup USB interface
        err = libusb_init(ctx);
        if (err < 0) {
            printf("Failed to initialize libusb: %s\n", libusb_error_name(err));
            return err;
        }
        if (!libusb_has_capability(LIBUSB_CAP_HAS_HOTPLUG)) {
            printf("LIBUSB reports that hotplug is not supported\n");
            libusb_exit(ctx);
            return -2;
        }
        err = libusb_hotplug_register_callback(
            ctx,
            LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED,
            LIBUSB_HOTPLUG_ENUMERATE,
            LIBUSB_HOTPLUG_MATCH_ANY,
            LIBUSB_HOTPLUG_MATCH_ANY,
            LIBUSB_HOTPLUG_MATCH_ANY,
            hotplug_callback_attach,
            NULL,
            NULL
        );
        if (err != LIBUSB_SUCCESS) {
            printf("Error registering usb attach callback: %s\n", libusb_error_name(err));
            return err;
        }
        err = libusb_hotplug_register_callback(
            ctx,
            LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT,
            LIBUSB_HOTPLUG_NO_FLAGS,
            LIBUSB_HOTPLUG_MATCH_ANY,
            LIBUSB_HOTPLUG_MATCH_ANY,
            LIBUSB_HOTPLUG_MATCH_ANY,
            hotplug_callback_detach,
            NULL,
            NULL
        );
        if (err != LIBUSB_SUCCESS) {
            printf("Error registering usb detach callback: %s\n", libusb_error_name(err));
            return err;
        }
        done = false;

        // Start calling the libusb event handler in another thread
        pthread_create(&recv_thread_id, NULL, &usb_recv_thread, NULL);
        return 0;
    }
    printf("Could not start usb thread, already running\n");
    return -1;
}

int ptp_usb_stop(void) {
    if (!done) {
        printf("Stopping libusb...\n");
        done = true;
        libusb_hotplug_deregister_callback(ctx, hotplug_callback_attach);
        libusb_hotplug_deregister_callback(ctx, hotplug_callback_detach);
        pthread_join(recv_thread_id, NULL);
        libusb_exit(ctx);
    }
}

void pack16(uint16_t val, unsigned char *buf) {
    buf[0] = val & 0xff;
    buf[1] = (val >> 8) & 0xff;
}

void pack32(uint32_t val, unsigned char *buf) {
    buf[0] = val & 0xff;
    buf[1] = (val >> 8) & 0xff;
    buf[2] = (val >> 16) & 0xff;
    buf[3] = (val >> 24) & 0xff;
}

uint32_t unpack32(unsigned char *buf) {
    uint32_t val = buf[3];
    val = (val << 8) + buf[2];
    val = (val << 8) + buf[1];
    val = (val << 8) + buf[0];
    return val;
}

uint16_t unpack16(unsigned char *buf) {
    uint16_t val = buf[1];
    val = (val << 8) + buf[0];
    return val;
}

int ptp_usb_transaction(command *cmd, libusb_device_handle *handle, void *callback) {
    struct libusb_transfer *transfer = libusb_alloc_transfer(0);
    unsigned char *buf;
    buf = malloc(32);
    pack32(cmd->length, buf);
    pack16(cmd->packet_type, buf+4);
    pack16(cmd->opcode, buf+6);
    pack32(cmd->transaction, buf+8);
    pack32(cmd->param1, buf+12);
    pack32(cmd->param2, buf+16);
    pack32(cmd->param3, buf+20);
    pack32(cmd->param4, buf+24);
    pack32(cmd->param5, buf+28);
    libusb_fill_bulk_transfer(transfer, handle, 0x02, buf, cmd->length, callback, (void *)cmd, 0);
    int ret = libusb_submit_transfer(transfer);
    if (LIBUSB_SUCCESS != ret) {
        printf("Couldn't submit transfer: %s\n", libusb_error_name(ret));
        print_trace();
    }
    return ret;
}

libusb_device_handle *ptp_usb_open(libusb_device *dev) {
    libusb_device_handle *handle;
    int ret;
    ret = libusb_open(dev, &handle);
    if (LIBUSB_SUCCESS != ret) {
        printf("Couldn't open usb device: %s\n", libusb_error_name(ret));
        return NULL;
    }
    return handle;
}
