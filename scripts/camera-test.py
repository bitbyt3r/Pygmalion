#!venv/bin/python

import pygmalion.ptp
import time

def camera_event(camera, event):
    print("Got event: {} from {}".format(event, camera))

def camera_added(camera):
    print("Found new camera: {}".format(camera))
    camera.register_event_cb(camera_event)
    camera.open()

def camera_removed(camera):
    print("Camera removed: {}".format(camera))

def camera_test(description):
    if description['vendor'] != 0x04A9:
        return False
    if description['product'] != 0x323B:
        return False
    return True

pygmalion.ptp.init(camera_added=camera_added, camera_removed=camera_removed, camera_test=camera_test)
time.sleep(5)