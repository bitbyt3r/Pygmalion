import pkgutil
import inspect
import importlib
from . import config
import sqlalchemy as sa
import sqlalchemy.sql.schema
import sqlalchemy.dialects.postgresql as psql

def create_table(module):
    tables = []
    for name, obj in inspect.getmembers(module):
        if type(obj) is sqlalchemy.sql.schema.Table:
            tables.append((str(sa.schema.CreateTable(obj).compile(dialect=psql.dialect())), "DROP TABLE {}".format(obj.name)))
    return tables

modules = []
pkgs = pkgutil.walk_packages(__path__)
for i in pkgs:
    module = importlib.import_module("{}.{}".format(__name__, i.name))
    modules.append(module)
    for name, obj in inspect.getmembers(module):
        if type(obj) is sqlalchemy.sql.schema.Table:
            globals()[name] = obj
