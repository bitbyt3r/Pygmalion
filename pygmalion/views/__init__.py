import pkgutil
import importlib
import functools
import time
import datetime
from sqlalchemy.sql import select, and_, text

from pygmalion.models import *

registrations = []

def register(*args, **kwargs):
    if args:
        if callable(args[0]):
            options = {}
            if 'options' in kwargs.keys():
                options = kwargs['options']
            name = ""
            if 'name' in kwargs.keys():
                name = kwargs['name']
            if len(args) > 1:
                name = args[1]
            if not name:
                name = args[0].__name__
            registrations.append({'method': args[0], 'name': name, 'options': options})
            return args[0]
        return functools.partial(register, name=args[0], **kwargs)
    return functools.partial(register, **kwargs)

class timer():
    def __init__(self, msg):
        self.msg = msg
    def __enter__(self):
        self.start = time.time()
    def __exit__(self,exc_type,exc_value,traceback):
        print(self.msg.format(int((time.time() - self.start)*1000)))
        
class PermissionDeniedException(Exception):
    pass

async def test_permission(conn, details, perm, session):
    if perm is str:
        db_perms = await conn.execute(Permission.select().where(and_(Permission.c.owner == session.user, Permission.c.name == perm)))
        db_perms = await db_perms.fetchall()
        if db_perms:
            return True
        db_perms = await conn.execute(select([Permission.c.uuid]).select_from(Permission.join(User_Group_Assoc, Permission.c.owner == User_Group_Assoc.c.group).where(User_Group_Assoc.c.user == session.User_Group_Assoc)))
        db_perms = await db_perms.fetchall()
        if db_perms:
            return True
        raise PermissionDeniedException
    else:
        # something like {"name": "Image.write", "oid": "<uuid4>"}
        db_perms = await conn.execute(Permission.select().where(and_(Permission.c.owner == session.user, Permission.c.name == perm['name'], Permission.c.oid == perm['oid'])))
        db_perms = await db_perms.fetchall()
        if db_perms:
            return True
        db_perms = await conn.execute(select([Permission.c.uuid]).select_from(Permission.join(User_Group_Assoc, Permission.c.owner == User_Group_Assoc.c.group).where(and_(User_Group_Assoc.c.user == session.User_Group_Assoc, Permission.c.oid == perm['oid']))))
        db_perms = await db_perms.fetchall()
        if db_perms:
            return True
        raise PermissionDeniedException

# If called with an empty permission list this will either return the user if logged in, or return a PermissionDeniedException if not.
async def check_permissions(conn, details, permissions=[]):
    sessions = await conn.execute(Session.select().where(Session.c.transport_id == details.caller))
    sessions = await sessions.fetchall()
    for i in sessions:
        end_time = datetime.datetime.now() + datetime.timedelta(30)
        current_time = datetime.datetime.strptime(i['time'], "%Y-%m-%d %H:%M:%S.%f")
        if end_time > current_time:
            for perm in permissions:
                await test_permission(conn, details, perm, i)
            return i.user
    raise PermissionDeniedException

pkgs = pkgutil.walk_packages(__path__)
for i in pkgs:
    importlib.import_module(__name__ + "." + i.name)
