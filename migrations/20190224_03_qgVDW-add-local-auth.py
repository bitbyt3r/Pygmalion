"""

"""

from yoyo import step

__depends__ = {'20190224_02_DMbyC-add-user-column'}

steps = [
    step("""CREATE EXTENSION "uuid-ossp" """,
         """DELETE EXTENSION "uuid-ossp" """
        ),
    step("""INSERT INTO auth_backends (uuid, name) VALUES (uuid_generate_v4(), 'Local')""", 
         """DELETE FROM auth_backends WHERE name='Local' """
        )

]
