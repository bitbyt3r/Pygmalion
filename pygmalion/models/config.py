from uuid import uuid4
import sqlalchemy as sa
import sqlalchemy.dialects.postgresql as psql

metadata = sa.MetaData()

def uuid():
    new_uuid = uuid4()
    return str(new_uuid)
