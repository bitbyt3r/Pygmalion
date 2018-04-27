"""
Creating base tables
"""

from yoyo import step

steps = [
    step("""
         
         CREATE TABLE auth_backends (
         	uuid UUID NOT NULL, 
         	name VARCHAR(255), 
         	settings JSON, 
         	PRIMARY KEY (uuid)
         )
         
         
         """,
         """DROP TABLE auth_backends"""
    ),
    step("""
         
         CREATE TABLE users (
         	uuid UUID NOT NULL, 
         	username VARCHAR(255), 
         	realname VARCHAR(255), 
         	email VARCHAR(255), 
         	auth_backend UUID, 
         	auth_data JSON, 
         	PRIMARY KEY (uuid), 
         	FOREIGN KEY(auth_backend) REFERENCES auth_backends (uuid)
         )
         
         
         """,
         """DROP TABLE users"""
    ),
    step("""
         
         CREATE TABLE sequences (
         	uuid UUID NOT NULL, 
         	name VARCHAR(255), 
         	"user" UUID, 
         	PRIMARY KEY (uuid), 
         	FOREIGN KEY("user") REFERENCES users (uuid)
         )
         
         
         """,
         """DROP TABLE sequences"""
    ),
    step("""
         
         CREATE TABLE captures (
         	uuid UUID NOT NULL, 
         	name VARCHAR(255), 
         	sequence UUID, 
         	"order" INTEGER, 
         	PRIMARY KEY (uuid), 
         	FOREIGN KEY(sequence) REFERENCES sequences (uuid)
         )
         
         
         """,
         """DROP TABLE captures"""
    ),
    step("""
         
         CREATE TABLE configurables (
         	uuid UUID NOT NULL, 
         	name VARCHAR(255), 
         	data JSON, 
         	PRIMARY KEY (uuid)
         )
         
         
         """,
         """DROP TABLE configurables"""
    ),
    step("""
         
         CREATE TABLE configurabletypes (
         	uuid UUID NOT NULL, 
         	name VARCHAR(255), 
         	data JSON, 
         	PRIMARY KEY (uuid)
         )
         
         
         """,
         """DROP TABLE configurabletypes"""
    ),
    step("""
         
         CREATE TABLE configurablegroups (
         	uuid UUID NOT NULL, 
         	configurabletype UUID, 
         	name VARCHAR(255), 
         	"user" UUID, 
         	PRIMARY KEY (uuid), 
         	FOREIGN KEY(configurabletype) REFERENCES configurabletypes (uuid), 
         	FOREIGN KEY("user") REFERENCES users (uuid)
         )
         
         
         """,
         """DROP TABLE configurablegroups"""
    ),
    step("""
         
         CREATE TABLE capturesettings (
         	uuid UUID NOT NULL, 
         	capture UUID, 
         	configurablegroup UUID, 
         	data JSON, 
         	PRIMARY KEY (uuid), 
         	FOREIGN KEY(capture) REFERENCES captures (uuid), 
         	FOREIGN KEY(configurablegroup) REFERENCES configurablegroups (uuid)
         )
         
         
         """,
         """DROP TABLE capturesettings"""
    ),
    step("""
         
         CREATE TABLE configurable_configurablegroup_assoc (
         	uuid UUID NOT NULL, 
         	configurable UUID, 
         	configurablegroup UUID, 
         	PRIMARY KEY (uuid), 
         	FOREIGN KEY(configurable) REFERENCES configurables (uuid), 
         	FOREIGN KEY(configurablegroup) REFERENCES configurablegroups (uuid)
         )
         
         
         """,
         """DROP TABLE configurable_configurablegroup_assoc"""
    ),
    step("""
         
         CREATE TABLE groupoptioncaches (
         	uuid UUID NOT NULL, 
         	configurablegroup UUID, 
         	data JSON, 
         	PRIMARY KEY (uuid), 
         	FOREIGN KEY(configurablegroup) REFERENCES configurablegroups (uuid)
         )
         
         
         """,
         """DROP TABLE groupoptioncaches"""
    ),
    step("""
         
         CREATE TABLE optioncaches (
         	uuid UUID NOT NULL, 
         	configurable UUID, 
         	data JSON, 
         	PRIMARY KEY (uuid), 
         	FOREIGN KEY(configurable) REFERENCES configurables (uuid)
         )
         
         
         """,
         """DROP TABLE optioncaches"""
    ),
    step("""
         
         CREATE TABLE settings (
         	uuid UUID NOT NULL, 
         	key VARCHAR(255), 
         	value VARCHAR(255), 
         	PRIMARY KEY (uuid)
         )
         
         
         """,
         """DROP TABLE settings"""
    ),
    step("""
         
         CREATE TABLE files (
         	uuid UUID NOT NULL, 
         	name VARCHAR(255), 
         	path VARCHAR(255), 
         	PRIMARY KEY (uuid)
         )
         
         
         """,
         """DROP TABLE files"""
    ),
    step("""
         
         CREATE TABLE folders (
         	uuid UUID NOT NULL, 
         	name VARCHAR(255), 
         	"user" UUID, 
         	PRIMARY KEY (uuid), 
         	FOREIGN KEY("user") REFERENCES users (uuid)
         )
         
         
         """,
         """DROP TABLE folders"""
    ),
    step("""
         
         CREATE TABLE groups (
         	uuid UUID NOT NULL, 
         	groupname VARCHAR(255), 
         	description VARCHAR(255), 
         	PRIMARY KEY (uuid)
         )
         
         
         """,
         """DROP TABLE groups"""
    ),
    step("""
         
         CREATE TABLE permissions (
         	uuid UUID NOT NULL, 
         	name VARCHAR(255), 
         	owner UUID, 
         	oid UUID, 
         	PRIMARY KEY (uuid)
         )
         
         
         """,
         """DROP TABLE permissions"""
    ),
    step("""
         
         CREATE TABLE sessions (
         	uuid UUID NOT NULL, 
         	"user" UUID, 
         	time TIMESTAMP WITHOUT TIME ZONE, 
         	transport_id INTEGER, 
         	PRIMARY KEY (uuid), 
         	FOREIGN KEY("user") REFERENCES users (uuid)
         )
         
         
         """,
         """DROP TABLE sessions"""
    ),
    step("""
         
         CREATE TABLE user_group_assoc (
         	uuid UUID NOT NULL, 
         	"user" UUID, 
         	"group" UUID, 
         	PRIMARY KEY (uuid), 
         	FOREIGN KEY("user") REFERENCES users (uuid), 
         	FOREIGN KEY("group") REFERENCES groups (uuid)
         )
         
         
         """,
         """DROP TABLE user_group_assoc"""
    ),
]

