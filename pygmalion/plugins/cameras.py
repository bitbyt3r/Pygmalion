"""A plugin that provides support for configuring and operating cameras

You probably want this in your camera rig, but I guess it's optional?
"""

from pygmalion.plugins import Plugin
import txaio

log = txaio.make_logger()

class Cameras(Plugin):
    # If you communicate with someone over the network then you can spin up your server here
    async def server(self):
        pass

    # Take the configuration and apply it. Once you are done, call the callback. You can pass your state 
    # to the callback if something went wrong and you think we should know
    async def configure(self, configuration, callback):
        pass

    # Return the options that go in the frontend form in some standard format
    async def get_form(self):
        return {
            "name": "Cameras",
            "settings": [
                {
                    "name": "Aperture",
                    "type": "select",
                },
                {
                    "name": "Shutter Speed",
                    "type": "select",
                },
                {
                    "name": "ISO",
                    "type": "select",
                },
                {
                    "name": "White Balance",
                    "type": "select",
                },
                {
                    "name": "Image Format",
                    "type": "select",
                }
            ],
        }

    # Return the options that are available for the given group
    async def get_options(self, configurable_group):
        pass
