"""
Create table to store capture flow graphs in
"""

from yoyo import step

__depends__ = {'20180424_01_Oi101-creating-base-tables'}

steps = [
    step("""
         
         CREATE TABLE capture_graphs (
         	uuid UUID NOT NULL, 
         	name VARCHAR(255), 
         	data JSON, 
         	PRIMARY KEY (uuid)
         )
         
         
         """,
         """DROP TABLE capture_graphs"""
    )
]
