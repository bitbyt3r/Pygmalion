#include <pthread.h>
#include "libusb.h"
#include "protocol.h"
#include <stdbool.h>
#include <sys/queue.h>
#include <stdio.h>
#include <glib.h>

bool done = true;
libusb_context *ctx = NULL;
pthread_t recv_thread_id;
pthread_t send_thread_id;

GHashTable *camera_table;
STAILQ_HEAD(stailhead, command) command_queue;

static void print_endpoint_comp(const struct libusb_ss_endpoint_companion_descriptor *ep_comp)
{
	printf("      USB 3.0 Endpoint Companion:\n");
	printf("        bMaxBurst:        %d\n", ep_comp->bMaxBurst);
	printf("        bmAttributes:     0x%02x\n", ep_comp->bmAttributes);
	printf("        wBytesPerInterval: %d\n", ep_comp->wBytesPerInterval);
}

static void print_endpoint(const struct libusb_endpoint_descriptor *endpoint)
{
	int i, ret;

	printf("      Endpoint:\n");
	printf("        bEndpointAddress: %02xh\n", endpoint->bEndpointAddress);
	printf("        bmAttributes:     %02xh\n", endpoint->bmAttributes);
	printf("        wMaxPacketSize:   %d\n", endpoint->wMaxPacketSize);
	printf("        bInterval:        %d\n", endpoint->bInterval);
	printf("        bRefresh:         %d\n", endpoint->bRefresh);
	printf("        bSynchAddress:    %d\n", endpoint->bSynchAddress);

	for (i = 0; i < endpoint->extra_length;) {
		if (LIBUSB_DT_SS_ENDPOINT_COMPANION == endpoint->extra[i + 1]) {
			struct libusb_ss_endpoint_companion_descriptor *ep_comp;

			ret = libusb_get_ss_endpoint_companion_descriptor(NULL, endpoint, &ep_comp);
			if (LIBUSB_SUCCESS != ret) {
				continue;
			}

			print_endpoint_comp(ep_comp);

			libusb_free_ss_endpoint_companion_descriptor(ep_comp);
		}

		i += endpoint->extra[i];
	}
}

static void print_altsetting(const struct libusb_interface_descriptor *interface)
{
	uint8_t i;

	printf("    Interface:\n");
	printf("      bInterfaceNumber:   %d\n", interface->bInterfaceNumber);
	printf("      bAlternateSetting:  %d\n", interface->bAlternateSetting);
	printf("      bNumEndpoints:      %d\n", interface->bNumEndpoints);
	printf("      bInterfaceClass:    %d\n", interface->bInterfaceClass);
	printf("      bInterfaceSubClass: %d\n", interface->bInterfaceSubClass);
	printf("      bInterfaceProtocol: %d\n", interface->bInterfaceProtocol);
	printf("      iInterface:         %d\n", interface->iInterface);

	for (i = 0; i < interface->bNumEndpoints; i++)
		print_endpoint(&interface->endpoint[i]);
}

static void print_interface(const struct libusb_interface *interface)
{
	int i;

	for (i = 0; i < interface->num_altsetting; i++)
		print_altsetting(&interface->altsetting[i]);
}

static void print_configuration(struct libusb_config_descriptor *config)
{
	uint8_t i;

	printf("  Configuration:\n");
	printf("    wTotalLength:         %d\n", config->wTotalLength);
	printf("    bNumInterfaces:       %d\n", config->bNumInterfaces);
	printf("    bConfigurationValue:  %d\n", config->bConfigurationValue);
	printf("    iConfiguration:       %d\n", config->iConfiguration);
	printf("    bmAttributes:         %02xh\n", config->bmAttributes);
	printf("    MaxPower:             %d\n", config->MaxPower);

	for (i = 0; i < config->bNumInterfaces; i++)
		print_interface(&config->interface[i]);
}

static int is_camera(struct libusb_device_descriptor *desc) {
    if (desc->idVendor != 0x04a9) {
        return 0;
    }
    if (desc->idProduct != 0x323b) {
        return 0;
    }
    return 1;
}

static int LIBUSB_CALL hotplug_callback_attach(libusb_context *ctx, libusb_device *dev, libusb_hotplug_event event, void *user_data) {
    struct libusb_device_descriptor *desc;
    desc = (struct libusb_device_descriptor*)malloc(sizeof(struct libusb_device_descriptor));
    int err;
    err = libusb_get_device_descriptor(dev, desc);
    if (err != LIBUSB_SUCCESS) {
        printf("Failed to get device descriptor\n");
        return 0;
    }
    printf("Device attached: %04x:%04x\n", desc->idVendor, desc->idProduct);
    if (!is_camera(desc)) {
        return 0;
    }

    camera *cam = (camera*)malloc(sizeof(camera));
    cam->desc = desc;
    cam->dev = dev;

    uint32_t* key = (uint32_t *)malloc(sizeof(uint32_t));
    uint32_t* val = (uint32_t *)malloc(sizeof(uint32_t));

    *key = dev;
    val = cam;
    g_hash_table_insert(camera_table, (gpointer)key, (gpointer)val);

    for (int i=0; i<cam->desc->bNumConfigurations; i++) {
        struct libusb_config_descriptor *config;
        int ret = libusb_get_config_descriptor(dev, i, &config);
        if (LIBUSB_SUCCESS != ret) {
            printf("  Couldn't retrieve descriptors\n");
            continue;
        }

        print_configuration(config);

        libusb_free_config_descriptor(config);
    }
    return 0;
}

static int LIBUSB_CALL hotplug_callback_detach(libusb_context *ctx, libusb_device *dev, libusb_hotplug_event event, void *user_data) {
    return 0;
}

void *usb_recv_thread(void *vargp) {
    while (!done) {
        libusb_handle_events(ctx);
    }
    pthread_exit(0);
}

void *usb_send_thread(void *vargp) {
    while (!done) {
        sleep(0.01);
        while (!STAILQ_EMPTY(&command_queue)) {
            command *cmd = STAILQ_FIRST(&command_queue);
            if (cmd != NULL) {
                STAILQ_REMOVE_HEAD(&command_queue, entry);

                free(cmd);
            }
        }
    }
    pthread_exit(0);
}

void ptp_usb_operation(command *cmd) {
    STAILQ_INSERT_TAIL(&command_queue, cmd, entry);
}

int ptp_usb_start(void) {
    int err = 0;
    if (done) {
        camera_table = g_hash_table_new(g_int_hash, g_int_equal);
        STAILQ_INIT(&command_queue);
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
        pthread_create(&recv_thread_id, NULL, &usb_recv_thread, NULL);
        pthread_create(&send_thread_id, NULL, &usb_send_thread, NULL);
        return 0;
    }
    printf("Could not start usb thread, already running\n");
    return -1;
}

int ptp_usb_stop(void) {
    if (!done) {
        done = true;
        libusb_hotplug_deregister_callback(ctx, hotplug_callback_attach);
        libusb_hotplug_deregister_callback(ctx, hotplug_callback_detach);
        pthread_join(recv_thread_id, NULL);
        pthread_join(send_thread_id, NULL);
        libusb_exit(ctx);
    }
}

GHashTable *ptp_usb_list_cameras(void) {
    return camera_table;
}

#define LEN_IN_BUFFER 1024*8
static uint8_t in_buffer[LEN_IN_BUFFER];

void read_cb(struct libusb_transfer *transfer) {
    printf("Read CB\n");
}

void write_cb(struct libusb_transfer *transfer) {
    if (transfer->status == LIBUSB_TRANSFER_COMPLETED) {
        printf("Transfer successful\n");
    } else {
        printf("Transfer failed: %d\n", transfer->status);
    }
    unsigned char *buf;
    buf = malloc(1024);
    int length=1024;
    libusb_fill_bulk_transfer(transfer, transfer->dev_handle, 0x81, buf, length, read_cb, NULL, 0);
    int ret;
    printf("Submitting transfer\n");
    ret = libusb_submit_transfer(transfer);
    if (LIBUSB_SUCCESS != ret) {
        printf("Couldn't submit transfer: %s\n", libusb_error_name(ret));
        return;
    }
    printf("Read submitted\n");
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

void ptp_usb_transaction(command *cmd, struct libusb_transfer *transfer) {
    unsigned char *buf;
    buf = malloc(24);
    int length = 24;
    int ptype  = 1;
    pack32(length, buf);
    pack16(ptype, buf+4);
    pack16(cmd->opcode, buf+6);
    pack32(cmd->transaction, buf+8);
    pack32(cmd->param1, buf+12);
    pack32(cmd->param2, buf+16);
    pack32(cmd->param3, buf+20);
    libusb_fill_bulk_transfer(transfer, cmd->handle, 0x02, buf, length, write_cb, NULL, 0);
}

void ptp_usb_open_camera(libusb_device *dev) {
    camera *cam;
    cam = g_hash_table_lookup(camera_table, &dev);
    printf("Opening camera %04x:%04x\n", cam->desc->idVendor, cam->desc->idProduct);

    int ret;
    ret = libusb_open(cam->dev, &cam->handle);
    if (LIBUSB_SUCCESS != ret) {
        printf("Couldn't open usb device\n");
        return;
    }

    struct libusb_transfer *transfer = libusb_alloc_transfer(0);
    command *cmd = (command*)malloc(sizeof(command));
    cmd->handle = cam->handle;
    cmd->opcode = OpenSession;
    ptp_usb_transaction(cmd, transfer);
    printf("checkpoint\n");
    ret = libusb_submit_transfer(transfer);
    printf("success\n");
    if (LIBUSB_SUCCESS != ret) {
        printf("Couldn't submit transfer: %s\n", libusb_error_name(ret));
        return;
    }
    printf("Finished sending transfer\n");
}

void callback_in (struct libusb_transfer *transfer) {
    printf("Received %d bytes\n", transfer->actual_length);
}