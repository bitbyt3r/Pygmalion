"""
Add user column to capture graph table
"""

from yoyo import step

__depends__ = {'20190224_01_fdOUr-create-capture-graphs'}

steps = [
    step("""ALTER TABLE capture_graphs ADD COLUMN "user" UUID""",
         """ALTER TABLE capture_graphs DROP COLUMN "user" """),
    step("""ALTER TABLE capture_graphs ADD CONSTRAINT user_fk FOREIGN KEY("user") REFERENCES users (uuid)""",
         """ALTER TABLE capture_graphs DROP CONSTRAINT user_fk""")
]
