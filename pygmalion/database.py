import os
import asyncio
import warnings
with warnings.catch_warnings():
    warnings.simplefilter("ignore")
    import aiopg.sa
from yoyo import read_migrations, get_backend

import txaio
log = txaio.make_logger()

async def initialize(config=None, callback=None):
    async with aiopg.sa.create_engine(user=config['db_user'],
                             database=config['db_database'],
                             host=config['db_host'],
                             password=config['db_pass']) as engine:

        ## Run Migrations
        log.info("Running Migrations")
        backend = get_backend('postgres://{}:{}@{}/{}'.format(config['db_user'], config['db_pass'], config['db_host'], config['db_database']))
        if os.path.isdir('./migrations'):
            migrations = read_migrations('./migrations')
        else:
            migrations = read_migrations('/usr/lib/pygmalion/migrations')
        backend.apply_migrations(backend.to_apply(migrations))

        if callback:
            await callback(engine=engine, config=config)
