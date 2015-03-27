DROP TABLE IF EXISTS object_type
;
CREATE TABLE object_type
(
	type TEXT NOT NULL,
	CONSTRAINT PK_object_type PRIMARY KEY (type)
)
;

DROP TABLE IF EXISTS relation_type
;
CREATE TABLE relation_type
(
	type  NOT NULL,
	CONSTRAINT PK_relation_type PRIMARY KEY (type)
)
;

DROP TABLE IF EXISTS schema_object
;
CREATE TABLE schema_object
(
	id_ TEXT NOT NULL,
	name TEXT NOT NULL,
	type TEXT,
	comment TEXT,
	revision TEXT NOT NULL,
	create_date TEXT NOT NULL,
	update_date TEXT NOT NULL,
	CONSTRAINT PK_schema_object PRIMARY KEY (id_)
)
;

DROP TABLE IF EXISTS schema_object_parameter
;
CREATE TABLE schema_object_parameter
(
	schema_object_id TEXT NOT NULL,
	pid TEXT NOT NULL,
	param_name TEXT NOT NULL,
	CONSTRAINT PK_schema_object_param PRIMARY KEY (schema_object_id, pid, param_name),
	CONSTRAINT FK_schema_object_parameter_schema_object FOREIGN KEY (schema_object_id)
		REFERENCES schema_object(id_) ,
	CONSTRAINT FK_schema_object_parameter_schema_parameter FOREIGN KEY (pid)
		REFERENCES schema_parameter(id_) 
)
;

DROP TABLE IF EXISTS schema_object_seq
;
CREATE TABLE schema_object_seq
(
	id_seed INTEGER NOT NULL,
	CONSTRAINT PK_schema_object_seq PRIMARY KEY (id_seed)
)
;

DROP TABLE IF EXISTS schema_parameter
;
CREATE TABLE schema_parameter
(
	id_ TEXT NOT NULL,
	type TEXT,
	default_array_size INTEGER NOT NULL DEFAULT 1,
	min_array_size INTEGER NOT NULL DEFAULT 1,
	max_array_size INTEGER NOT NULL DEFAULT 1,
	comment TEXT,
	revision TEXT NOT NULL,
	create_date TEXT NOT NULL,
	update_date TEXT NOT NULL,
	basetype_name INTEGER NOT NULL,
	CONSTRAINT PK_schema_parameter PRIMARY KEY (id_)
)
;

DROP TABLE IF EXISTS schema_parameter_basetype_integer
;
CREATE TABLE schema_parameter_basetype_integer
(
	id_ TEXT NOT NULL,
	default_value INTEGER NOT NULL DEFAULT 0,
	system_min INTEGER NOT NULL DEFAULT -9223372036854775808,
	system_max INTEGER NOT NULL DEFAULT 9223372036854775807,
	warn_min INTEGER NOT NULL DEFAULT -9223372036854775808,
	warn_max INTEGER NOT NULL DEFAULT 9223372036854775807,
	CONSTRAINT PK_schema_parameter_basetype_integer PRIMARY KEY (id_),
	CONSTRAINT FK_schema_parameter_basetype_integer_schema_parameter FOREIGN KEY (id_)
		REFERENCES schema_parameter(id_) 
)
;

DROP TABLE IF EXISTS schema_parameter_basetype_real
;
CREATE TABLE schema_parameter_basetype_real
(
	id_ TEXT NOT NULL,
	default_value REAL NOT NULL DEFAULT 0,
	system_min REAL NOT NULL,
	system_max REAL NOT NULL,
	warn_min REAL NOT NULL,
	warn_max REAL NOT NULL,
	CONSTRAINT PK_schema_param_real PRIMARY KEY (id_)
)
;

DROP TABLE IF EXISTS schema_parameter_basetype_select
;
CREATE TABLE schema_parameter_basetype_select
(
	id_ TEXT NOT NULL,
	default_item_index INTEGER NOT NULL DEFAULT 0,
	enable_multi_select INTEGER NOT NULL DEFAULT 0,
	CONSTRAINT PK_schema_param_integer PRIMARY KEY (id_),
	CONSTRAINT FK_schema_parameter_basetype_select_schema_parameter FOREIGN KEY (id_)
		REFERENCES schema_parameter(id_) 
)
;

DROP TABLE IF EXISTS schema_parameter_basetype_select_item
;
CREATE TABLE schema_parameter_basetype_select_item
(
	id_ TEXT NOT NULL,
	item_index INTEGER NOT NULL,
	item_name TEXT DEFAULT 0,
	comment INTEGER,
	CONSTRAINT PK_schema_param_integer PRIMARY KEY (id_, item_index),
	CONSTRAINT FK_schema_parameter_basetype_selection_item_schema_parameter_basetype_selection FOREIGN KEY (id_)
		REFERENCES schema_parameter_basetype_select(id_) 
)
;

DROP TABLE IF EXISTS schema_parameter_basetype_text
;
CREATE TABLE schema_parameter_basetype_text
(
	id_ TEXT NOT NULL,
	default_value TEXT,
	system_min INTEGER NOT NULL DEFAULT 0,
	system_max INTEGER NOT NULL DEFAULT 9999,
	warn_min INTEGER NOT NULL DEFAULT 0,
	warn_max INTEGER NOT NULL DEFAULT 9999,
	CONSTRAINT PK_schema_param_text PRIMARY KEY (id_)
)
;

DROP TABLE IF EXISTS schema_parameter_seq
;
CREATE TABLE schema_parameter_seq
(
	id_seed INTEGER NOT NULL,
	CONSTRAINT PK_schema_param_seq PRIMARY KEY (id_seed)
)
;

DROP TABLE IF EXISTS schema_relation
;
CREATE TABLE schema_relation
(
	id_ TEXT NOT NULL,
	name TEXT,
	type TEXT NOT NULL,
	from_id TEXT NOT NULL,
	to_id TEXT NOT NULL,
	from_multiplicity_min INTEGER NOT NULL DEFAULT 1,
	from_multiplicity_max INTEGER NOT NULL DEFAULT 1,
	to_multiplicity_min INTEGER NOT NULL DEFAULT 0,
	to_multiplicity_max INTEGER NOT NULL DEFAULT 0,
	comment TEXT,
	revision TEXT NOT NULL DEFAULT 0,
	create_date TEXT NOT NULL,
	update_date TEXT NOT NULL,
	CONSTRAINT PK_schema_relatoin PRIMARY KEY (id_),
	CONSTRAINT FK_schm_rel_schm_obj1 FOREIGN KEY (from_id)
		REFERENCES schema_object(id_) ,
	CONSTRAINT FK_schm_rel_schm_obj2 FOREIGN KEY (to_id)
		REFERENCES schema_object(id_) 
)
;

DROP TABLE IF EXISTS schema_relation_parameter
;
CREATE TABLE schema_relation_parameter
(
	schema_relation_id TEXT NOT NULL,
	pid TEXT NOT NULL,
	param_name TEXT NOT NULL,
	CONSTRAINT PK_schema_relation_parameter PRIMARY KEY (schema_relation_id, pid, param_name),
	CONSTRAINT FK_schema_relation_parameter_schema_parameter FOREIGN KEY (pid)
		REFERENCES schema_parameter(id_) ,
	CONSTRAINT FK_schema_relation_parameter_schema_relation FOREIGN KEY (schema_relation_id)
		REFERENCES schema_relation(id_) 
)
;

DROP TABLE IF EXISTS schema_relation_seq
;
CREATE TABLE schema_relation_seq
(
	id_seed INTEGER NOT NULL,
	CONSTRAINT PK_schema_relation_seq PRIMARY KEY (id_seed)
)
;

DROP TABLE IF EXISTS session_object
;
CREATE TABLE session_object
(
	id_ TEXT NOT NULL,
	schema_object_id TEXT NOT NULL,
	name TEXT,
	comment TEXT,
	revision TEXT NOT NULL,
	create_date TEXT NOT NULL,
	update_date TEXT NOT NULL,
	CONSTRAINT PK_sessoin_object PRIMARY KEY (id_),
	CONSTRAINT FK_session_object_schema_object FOREIGN KEY (schema_object_id)
		REFERENCES schema_object(id_) 
)
;

DROP TABLE IF EXISTS session_object_parameter_basetype_integer
;
CREATE TABLE session_object_parameter_basetype_integer
(
	oid TEXT NOT NULL,
	param_name TEXT NOT NULL,
	array_index INTEGER NOT NULL,
	value INTEGER NOT NULL,
	CONSTRAINT PK_session_object_parameter_basetype_integer PRIMARY KEY (oid, param_name, array_index),
	CONSTRAINT FK_session_object_parameter_basetype_interger_session_object FOREIGN KEY (oid)
		REFERENCES session_object(id_) ON DELETE CASCADE ON UPDATE CASCADE
)
;

DROP TABLE IF EXISTS session_object_parameter_basetype_real
;
CREATE TABLE session_object_parameter_basetype_real
(
	oid TEXT NOT NULL,
	param_name TEXT NOT NULL,
	array_index INTEGER NOT NULL,
	value REAL,
	CONSTRAINT PK_session_object_parameter_basetype_real PRIMARY KEY (oid, param_name, array_index),
	CONSTRAINT FK_session_object_parameter_basetype_real_session_object FOREIGN KEY (oid)
		REFERENCES session_object(id_) 
)
;

DROP TABLE IF EXISTS session_object_parameter_basetype_select
;
CREATE TABLE session_object_parameter_basetype_select
(
	oid TEXT NOT NULL,
	param_name TEXT NOT NULL,
	array_index INTEGER NOT NULL,
	value INTEGER,
	CONSTRAINT PK_session_object_parameter_basetype_select PRIMARY KEY (oid, param_name, array_index)
)
;

DROP TABLE IF EXISTS session_object_parameter_basetype_text
;
CREATE TABLE session_object_parameter_basetype_text
(
	oid TEXT NOT NULL,
	param_name TEXT NOT NULL,
	array_index INTEGER NOT NULL,
	value TEXT,
	CONSTRAINT PK_session_object_parameter_basetype_text PRIMARY KEY (oid, param_name, array_index),
	CONSTRAINT FK_session_object_parameter_basetype_text_session_object FOREIGN KEY (oid)
		REFERENCES session_object(id_) 
)
;

DROP TABLE IF EXISTS session_object_seq
;
CREATE TABLE session_object_seq
(
	id_seed INTEGER NOT NULL,
	CONSTRAINT PK_session_object_seq PRIMARY KEY (id_seed)
)
;

DROP TABLE IF EXISTS session_relation
;
CREATE TABLE session_relation
(
	id_ TEXT NOT NULL,
	schema_relation_id TEXT,
	name TEXT,
	from_id TEXT NOT NULL,
	to_id TEXT NOT NULL,
	comment TEXT,
	revision TEXT NOT NULL,
	create_date TEXT NOT NULL,
	update_date TEXT NOT NULL,
	CONSTRAINT PK_session_relation PRIMARY KEY (id_),
	CONSTRAINT FK_session_relation_schema_relation FOREIGN KEY (schema_relation_id)
		REFERENCES schema_relation(id_) ,
	CONSTRAINT FK_session_relation_session_object FOREIGN KEY (to_id)
		REFERENCES session_object(id_) ,
	CONSTRAINT FK_session_relation_from_id_session_object_id FOREIGN KEY (from_id)
		REFERENCES session_object(id_) 
)
;

DROP TABLE IF EXISTS session_relation_parameter_basetype_integer
;
CREATE TABLE session_relation_parameter_basetype_integer
(
	relid TEXT NOT NULL,
	param_name TEXT NOT NULL,
	array_index INTEGER NOT NULL,
	value INTEGER NOT NULL,
	CONSTRAINT PK_sessopm_parameter_basetype_interger PRIMARY KEY (relid, param_name, array_index),
	CONSTRAINT FK_session_relation_parameter_basetype_interger_session_relation FOREIGN KEY (relid)
		REFERENCES session_relation(id_) ON DELETE CASCADE ON UPDATE CASCADE
)
;

DROP TABLE IF EXISTS session_relation_parameter_basetype_real
;
CREATE TABLE session_relation_parameter_basetype_real
(
	relid TEXT NOT NULL,
	param_name TEXT NOT NULL,
	array_index INTEGER NOT NULL,
	value REAL,
	CONSTRAINT PK_session_relation_parameter_basetype_real PRIMARY KEY (relid, param_name, array_index),
	CONSTRAINT FK_session_relation_parameter_basetype_real_session_relation FOREIGN KEY (relid)
		REFERENCES session_relation(id_) 
)
;

DROP TABLE IF EXISTS session_relation_parameter_basetype_select
;
CREATE TABLE session_relation_parameter_basetype_select
(
	relid TEXT NOT NULL,
	param_name TEXT NOT NULL,
	array_index INTEGER NOT NULL,
	value INTEGER,
	CONSTRAINT PK_session_relation_parameter_basetype_select PRIMARY KEY (relid, param_name, array_index)
)
;

DROP TABLE IF EXISTS session_relation_parameter_basetype_text
;
CREATE TABLE session_relation_parameter_basetype_text
(
	relid TEXT NOT NULL,
	param_name TEXT NOT NULL,
	array_index INTEGER NOT NULL,
	value TEXT,
	CONSTRAINT PK_session_object_parameter_basetype_text PRIMARY KEY (relid, param_name, array_index),
	CONSTRAINT FK_session_relation_parameter_basetype_text_session_relation FOREIGN KEY (relid)
		REFERENCES session_relation(id_) 
)
;

DROP TABLE IF EXISTS session_relation_seq
;
CREATE TABLE session_relation_seq
(
	id_seed INTEGER NOT NULL,
	CONSTRAINT PK_session_relation_seq PRIMARY KEY (id_seed)
)
;
