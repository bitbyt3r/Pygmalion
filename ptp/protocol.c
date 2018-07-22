#include <pthread.h>
#include "libusb.h"
#include "protocol.h"
#include <stdbool.h>

bool done = true;
libusb_context *ctx = NULL;
pthread_t thread_id;

static int LIBUSB_CALL hotplug_callback_attach(libusb_context *ctx, libusb_device *dev, libusb_hotplug_event event, void *user_data) {
    struct libusb_device_descriptor desc;
    int err;
    err = libusb_get_device_descriptor(dev, &desc);
    if (err != LIBUSB_SUCCESS) {
        printf("Failed to get device descriptor\n");
        return 0;
    }
    printf("Device attached: %04x:%04x\n", desc.idVendor, desc.idProduct);
    return 0;
}

static int LIBUSB_CALL hotplug_callback_detach(libusb_context *ctx, libusb_device *dev, libusb_hotplug_event event, void *user_data) {
    return 0;
}

void *usb_thread(void *vargp) {
    while (!done) {
        libusb_handle_events(ctx);
    }
}

int ptp_usb_start(void) {
    int err = 0;
    if (done) {
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
        err = pthread_create(&thread_id, NULL, usb_thread, NULL);
        return err;
    }
    printf("Could not start usb thread, already running\n");
    return -1;
}

int ptp_usb_stop(void) {
    if (!done) {
        done = true;
        libusb_hotplug_deregister_callback(ctx, hotplug_callback_attach);
        libusb_hotplug_deregister_callback(ctx, hotplug_callback_detach);
        pthread_join(&thread_id, NULL);
        libusb_exit(ctx);
    }
}
