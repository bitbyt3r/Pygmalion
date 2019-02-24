import sqlalchemy as sa
from . import config
import sqlalchemy.dialects.postgresql as psql

# A list of captures owned by a user
CaptureGraph = sa.Table('capture_graphs', config.metadata,
    sa.Column('uuid', psql.UUID, primary_key=True, default=config.uuid),
    sa.Column('name', sa.String(255)),
    sa.Column('data', sa.JSON),
    sa.Column('user', sa.ForeignKey('users.uuid')),
)