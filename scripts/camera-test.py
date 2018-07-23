#!venv/bin/python

import pygmalion.ptp
import time

print("Initializing library")
pygmalion.ptp.init()
print("Listing cameras")
cameras = pygmalion.ptp.list_cameras()
for i in cameras:
    print("Opening camera: ", i)
    pygmalion.ptp.open_camera(i['device'])
time.sleep(10)
print("Closing library")
pygmalion.ptp.stop()
print("Done!")
