import sqlalchemy as sa
from . import config
import sqlalchemy.dialects.postgresql as psql

# A list of captures owned by a user
Sequence = sa.Table('sequences', config.metadata,
    sa.Column('uuid', psql.UUID, primary_key=True, default=config.uuid),
    sa.Column('name', sa.String(255)),
    sa.Column('user', sa.ForeignKey('users.uuid')),
)

# Everything needed to generate a list of images
Capture = sa.Table('captures', config.metadata,
    sa.Column('uuid', psql.UUID, primary_key=True, default=config.uuid),
    sa.Column('name', sa.String(255)),
    sa.Column('sequence', sa.ForeignKey('sequences.uuid')),
    sa.Column('order', sa.Integer()),
)

# A list of types that can be configured. Probably just ['flashes', 'cameras', 'projectors', 'triggerboxes']
ConfigurableType = sa.Table('configurabletypes', config.metadata,
    sa.Column('uuid', psql.UUID, primary_key=True, default=config.uuid),
    sa.Column('name', sa.String(255)),
    sa.Column('data', sa.JSON),
)

# A group of configurable items, such as a set of cameras or flashes
ConfigurableGroup = sa.Table('configurablegroups', config.metadata,
    sa.Column('uuid', psql.UUID, primary_key=True, default=config.uuid),
    sa.Column('configurabletype', sa.ForeignKey('configurabletypes.uuid')),
    sa.Column('name', sa.String(255)),
    sa.Column('user', sa.ForeignKey('users.uuid')),
)

# A single configurable item (such as a single camera or projector)
Configurable = sa.Table('configurables', config.metadata,
    sa.Column('uuid', psql.UUID, primary_key=True, default=config.uuid),
    sa.Column('name', sa.String(255)),
    sa.Column('data', sa.JSON),
)

# Links configurables into their groups
Configurable_ConfigurableGroup_Assoc = sa.Table('configurable_configurablegroup_assoc', config.metadata,
    sa.Column('uuid', psql.UUID, primary_key=True, default=config.uuid),
    sa.Column('configurable', sa.ForeignKey('configurables.uuid')),
    sa.Column('configurablegroup', sa.ForeignKey('configurablegroups.uuid')),
)

# All settings needed to configure configurable group for this capture. This might hold all camera settings for a group of cameras.
CaptureSetting = sa.Table('capturesettings', config.metadata,
    sa.Column('uuid', psql.UUID, primary_key=True, default=config.uuid),
    sa.Column('capture', sa.ForeignKey('captures.uuid')),
    sa.Column('configurablegroup', sa.ForeignKey('configurablegroups.uuid')),
    sa.Column('data', sa.JSON),
)

# Cache a set of options provided by an individual configurable
OptionCache = sa.Table('optioncaches', config.metadata,
    sa.Column('uuid', psql.UUID, primary_key=True, default=config.uuid),
    sa.Column('configurable', sa.ForeignKey('configurables.uuid')),
    sa.Column('data', sa.JSON),
)

# A cache for all settings supported by a group of configurables
GroupOptionCache = sa.Table('groupoptioncaches', config.metadata,
    sa.Column('uuid', psql.UUID, primary_key=True, default=config.uuid),
    sa.Column('configurablegroup', sa.ForeignKey('configurablegroups.uuid')),
    sa.Column('data', sa.JSON),
)

