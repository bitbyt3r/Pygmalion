import sqlalchemy as sa
from . import config
import sqlalchemy.dialects.postgresql as psql

User = sa.Table('users', config.metadata,
    sa.Column('uuid', psql.UUID, primary_key=True, default=config.uuid),
    sa.Column('username', sa.String(255)),
    sa.Column('realname', sa.String(255)),
    sa.Column('email', sa.String(255)),
    sa.Column('auth_backend', sa.ForeignKey('auth_backends.uuid')),
    sa.Column('auth_data', sa.JSON),
)

Group = sa.Table('groups', config.metadata,
    sa.Column('uuid', psql.UUID, primary_key=True, default=config.uuid),
    sa.Column('groupname', sa.String(255)),
    sa.Column('description', sa.String(255)),
)

User_Group_Assoc = sa.Table('user_group_assoc', config.metadata,
    sa.Column('uuid', psql.UUID, primary_key=True, default=config.uuid),
    sa.Column('user', sa.ForeignKey('users.uuid')),
    sa.Column('group', sa.ForeignKey('groups.uuid')),
)

Auth_Backend = sa.Table('auth_backends', config.metadata,
    sa.Column('uuid', psql.UUID, primary_key=True, default=config.uuid),
    sa.Column('name', sa.String(255)),
    sa.Column('settings', sa.JSON),
)

Permission = sa.Table('permissions', config.metadata,
    sa.Column('uuid', psql.UUID, primary_key=True, default=config.uuid),
    sa.Column('name', sa.String(255)),
    sa.Column('owner', psql.UUID), # Could be a group or a user
    sa.Column('oid', psql.UUID), # Could be any other object
)

Session = sa.Table('sessions', config.metadata,
    sa.Column('uuid', psql.UUID, primary_key=True, default=config.uuid),
    sa.Column('user', sa.ForeignKey('users.uuid')),
    sa.Column('time', sa.DateTime),
    sa.Column('transport_id', sa.Integer),
)
