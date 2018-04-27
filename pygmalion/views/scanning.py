from pygmalion.models import *
from pygmalion.views import register, check_permissions
from pygmalion.configure import config
from pygmalion.plugins import instances
from sqlalchemy.sql import select, and_, text
import txaio

log = txaio.make_logger()

@register('load_scan_page', options={'details_arg': 'details'})
async def load_scan_page(engine, details=None):
    async with engine.acquire() as conn:
        async with conn.begin() as tr:
            user = check_permissions(conn, details)
            sequences = await conn.execute(Sequence.select().where(Sequence.c.user == user.uuid))
            sequences = await sequences.fetchall()
            captures = await conn.execute(Capture.select().where(Capture.c.sequence.in_([x.uuid for x in sequences])))
            captures = await captures.fetchall()

            return {"sequences": sequences, "captures": captures}
