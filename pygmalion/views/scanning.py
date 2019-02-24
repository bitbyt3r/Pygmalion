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
            user = await check_permissions(conn, details)
            sequences = await conn.execute(Sequence.select().where(Sequence.c.user == user))
            sequences = await sequences.fetchall()
            captures = await conn.execute(Capture.select().where(Capture.c.sequence.in_([x.uuid for x in sequences])))
            captures = await captures.fetchall()

            return {"sequences": sequences, "captures": captures}

@register('get_capture_graphs', options={'details_arg': 'details'})
async def get_capture_graphs(engine, details=None):
    async with engine.acquire() as conn:
        user = await check_permissions(conn, details)
        capture_graphs = await conn.execute(CaptureGraph.select().where(CaptureGraph.c.user == user))
        capture_graphs = await capture_graphs.fetchall()
        return {"capture_graphs": capture_graphs}