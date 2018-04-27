import sqlalchemy as sa
from . import config
import sqlalchemy.dialects.postgresql as psql

Setting = sa.Table('settings', config.metadata,
    sa.Column('uuid', psql.UUID, primary_key=True, default=config.uuid),
    sa.Column('key', sa.String(255)),
    sa.Column('value', sa.String(255)),
)
