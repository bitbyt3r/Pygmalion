import os
import sys
import json
import txaio

log = txaio.make_logger()

config = {
            "db_user": "pygmalion",
            "db_pass": "password",
            "db_host": "localhost",
            "db_database": "pygmalion",
            "wamp_url": "ws://localhost:8080/ws",
            "wamp_host": "localhost",
            "wamp_port": 8080,
        }

def get_config(path=""):
    if not path:
        path = "/etc/pygmalion/conf.json"
        if not os.path.isfile(path):
            log.info("No config file provided. Continuing with defaults.")
            return
    elif not path:
        sys.exit("{} is not a file. Could not load config.".format(path))
    try:
        with open(path, "r") as FILE:
            newconfig = json.load(FILE)
    except:
        sys.exit("Could not parse config file: {}".format(path))
    config.update(newconfig)
    return
