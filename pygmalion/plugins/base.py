class Plugin:
    # Init is a good place to do basic configuration from the database
    def __init__(self, engine=None, config={}):
        self.config = config
        self.engine = engine

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
            "name": "Basic Plugin",
            "settings": [
                {
                    "name": "Text Option",
                    "type": "text",
                    "default": "You can enter text here"
                },
                {
                    "name": "Selection Option",
                    "type": "select",
                    "default": None
                },
                {
                    "name": "Checkbox",
                    "type": "check",
                    "default": True
                }
            ],
        }

    # Return the options that are available for the given group
    async def get_options(self, configurable_group):
        return {
            "Selection Option": [
                "Item 1",
                "Item 2",
                "Item 3",
            ]
        }
