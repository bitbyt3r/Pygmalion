#!/usr/bin/python3

import pygmalion.ptp
import time

cameras = []

def camera_event(camera, event):
    print("Got event: {} from {}".format(event, camera))

def camera_added(camera):
    camera.register_event_cb(camera_event)
    camera.open()
    cameras.append(camera)

def camera_removed(camera):
    print("Camera removed: {}".format(camera))

# 04a9:3272 - Canon T5i
# 04a9:323b - Canon T4i
def camera_test(description):
    if description['vendor'] != 0x04A9:
        return False
    if description['product'] in [0x323b, 0x3272]:
        return True
    return False

pygmalion.ptp.init(camera_added=camera_added, camera_removed=camera_removed, camera_test=camera_test)
time.sleep(60)
print("Closing cameras...")
for camera in cameras:
    camera.close()
