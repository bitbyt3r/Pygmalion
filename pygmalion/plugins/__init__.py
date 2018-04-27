import pkgutil
import inspect
import importlib
from pygmalion.plugins.base import Plugin

plugins = []
instances = []

pkgs = pkgutil.walk_packages(__path__)
for i in pkgs:
    module = importlib.import_module("{}.{}".format(__name__, i.name))
    for name, obj in inspect.getmembers(module):
        if inspect.isclass(obj):
            if name == "Plugin":
                continue
            globals()[name] = obj
            plugins.append(obj)

def start(*arg, **kwargs):
    for plugin in plugins:
        instances.append(plugin(*arg, **kwargs))