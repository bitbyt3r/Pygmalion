import sqlalchemy as sa
from . import config
import sqlalchemy.dialects.postgresql as psql

Folder = sa.Table('folders', config.metadata,
    sa.Column('uuid', psql.UUID, primary_key=True, default=config.uuid),
    sa.Column('name', sa.String(255)),
    sa.Column('user', sa.ForeignKey('users.uuid')),
)

File = sa.Table('files', config.metadata,
    sa.Column('uuid', psql.UUID, primary_key=True, default=config.uuid),
    sa.Column('name', sa.String(255)),
    sa.Column('path', sa.String(255)),
)