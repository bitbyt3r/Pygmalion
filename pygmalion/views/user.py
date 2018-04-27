from pygmalion.models import *
from pygmalion.views import register
from pygmalion.configure import config
from sqlalchemy.sql import select, and_, text

import txaio
import aiohttp
import datetime
import requests
from passlib.hash import sha256_crypt
import uuid

log = txaio.make_logger()

async def validate_recaptcha(recaptcha):
    async with aiohttp.ClientSession() as session:
        async with session.get("https://www.google.com/recaptcha/api/siteverify", params={"secret": config.get('recaptcha-secret'), "response": recaptcha}) as resp:
            success = await resp.json()
            log.info(success)
            return success['success']

@register('session.login', options={'details_arg': 'details'})
async def login(engine, username=None, password=None, details=None):
    async with engine.acquire() as conn:
        async with conn.begin() as tr:
            user = await conn.execute(User.select().where(User.c.username == username))
            user = await user.fetchall()
            if len(user) == 1:
                user = user[0]
            else:
                log.error("Username {} not found.".format(username))
                return {"success": False}
            auth = await conn.execute(Auth_Backend.select().where(Auth_Backend.c.uuid == user.auth_backend))
            auth = await auth.fetchall()
            if len(auth) == 1:
                auth = auth[0]
            else:
                log.error("Auth_Backend {} not found while logging in user {}.".format(user.auth_backend, username))
                return {"success": False}
            if auth.name == 'Local':
                if sha256_crypt.verify(password, user.auth_data):
                    session = await conn.execute(Session.insert().values(user=user.uuid, time=datetime.datetime.now(), transport_id=details.caller).returning(Session.c.uuid))
                    session = await session.fetchone()
                    return {"success": True, "session": session.uuid, "user": user}
            log.error("Login failed for user {}".format(username))
            return False
                
@register('session.logout', options={'details_arg': 'details'})
async def logout(engine, session=None, details=None):
    async with engine.acquire() as conn:
        sessions = await conn.execute(Session.delete().where(Session.c.uuid == session))
        return {"success": True}

@register('session.renew', options={'details_arg': 'details'})
async def renew(engine, session=None, details=None):
    log.info("Running renew...")
    async with engine.acquire() as conn:
        async with conn.begin() as tr:
            current_session = await conn.execute(Session.select().where(Session.c.uuid == session))
            current_session = await current_session.fetchone()
            print(current_session)
            if (current_session):
                if datetime.datetime.now() + datetime.timedelta(30) > current_session.time:
                    await conn.execute(Session.update().where(Session.c.uuid == session).values(time=datetime.datetime.now()))
                    user = await conn.execute(User.select().where(User.c.uuid == current_session.user))
                    user = await user.fetchone()
                    log.info("Renewed session successfully")
                    return {"success": True, "user": user}
            log.info("Failed to renew session")
            return {"success": False}

# TODO: Run date comparisons in the database
@register('session.verify', options={'details_arg': 'details'})
async def verify(engine, details=None):
    async with engine.acquire() as conn:
        sessions = await conn.execute(Session.select().where(Session.c.transport_id == details.caller))
        sessions = await sessions.fetchall()
        for i in sessions:
            end_time = datetime.datetime.now() + datetime.timedelta(30)
            current_time = datetime.datetime.strptime(i['time'], "%Y-%m-%d %H:%M:%S.%f")
            if end_time > current_time:
                return True
        return False

@register('user.register', options={'details_arg': 'details'})
async def user_register(engine, details=None, username="", fullname="", password="", terms="", email="", recaptcha=""):
    async with engine.acquire() as conn:
        user = await conn.execute(User.select().where(User.c.username == username))
        user = await user.fetchone()
        if user:
            return {"success": False, "reason": "A user named {} already exists.".format(username)}
        if not username:
            return {"success": False, "reason": "You must provide a username."}
        if not fullname:
            return {"success": False, "reason": "You must provide your full name."}
        if not password:
            return {"success": False, "reason": "You must provide a password."}
        if len(password) < 6:
            return {"success": False, "reason": "Your password must be at least 6 characters long."}
        if not terms:
            return {"success": False, "reason": "You must accept the terms of use."}
        if not await validate_recaptcha(recaptcha):
            return {"success": False, "reason": "You must prove that you are not a robot."}
        hashstr = sha256_crypt.encrypt(password)
        auth_backend = await conn.execute(Auth_Backend.select().where(Auth_Backend.c.name == "Local"))
        auth_backend = await auth_backend.fetchone()
        if not auth_backend:
            log.error("Could not locate Local auth backend while registering user {}".format(username))
            return {"success": False}
        await conn.execute(User.insert().values(username=username, realname=fullname, auth_data=hashstr, email=email, auth_backend=auth_backend.uuid))
        return {"success": True}

@register('user.update', options={'details_arg': 'details'})
async def user_update(engine, loggedinUser="", username="", fullname="", email="", details=None):
    async with engine.acquire() as conn:
        log.info("updating user")
        user = await conn.execute(User.select().where(User.c.username == loggedinUser))
        user = await user.fetchone()
        if not user:
            log.error("No user found named {}".format(loggedinUser))
            return {"success": False, "reason": "No user currently named {} exists.".format(loggedinUser)}
        if loggedinUser != username:
            currentuser = await conn.execute(User.select().where(User.c.username == username))
            currentuser = await currentuser.fetchall()
            if currentuser:
                log.error("Username {} is already in use.".format(username))
                return {"success": False, "reason": "The username {} is already in use.".format(username)}
        if not fullname:
            log.error("Users must have a full name.")
            return {"success": False, "reason": "You must provide your full name."}
        if not email:
            log.error("Users must provide an email address")
            return {"success": False, "reason": "You must provide you email address."}
        if not '@' in email:
            log.error("Email address is invalid")
            return {"success": False, "reason": "Your email address, {}, does not appear to be valid.".format(email)}
        await conn.execute(User.update().where(User.c.username == user.username).values(username=username, realname=fullname, email=email))
        log.info("User update successful")
        return {"success": True}

@register('user.change_password', options={'details_arg': 'details'})
async def user_change_password(engine, username="", password="", newPassword="", details=None):
    async with engine.acquire() as conn:
        user = await conn.execute(User.select().where(User.c.username == username))
        user = await user.fetchone()
        if not user:
            return {"success": False}
        auth_backend = await conn.execute(Auth_Backend.select().where(Auth_Backend.c.uuid == user.auth_backend))
        auth_backend = await auth_backend.fetchone()
        if auth_backend.name == "Local":
            if sha256_crypt.verify(password, user.auth_data):
                newHash = sha256_crypt.encrypt(newPassword)
                await conn.execute(User.update().where(User.c.uuid == user.uuid).values(auth_data=newHash))
                return {"success": True}
        return {"success": False}
