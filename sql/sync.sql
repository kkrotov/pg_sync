CREATE SCHEMA event  AUTHORIZATION postgres;

CREATE OR REPLACE FUNCTION event.queue_insert()
  RETURNS trigger AS
$BODY$
BEGIN
    --RAISE NOTICE 'server_id=%',NEW.server_id;
    --RAISE NOTICE 'event=%',NEW.event;
    --RAISE NOTICE 'param=%',NEW.param;
    
    IF EXISTS (SELECT 1 FROM event.queue WHERE server_id=NEW.server_id AND event=NEW.event AND param=NEW.param) THEN
        --RAISE NOTICE 'FOUND';
        RETURN NULL;
    END IF;
    
    --RAISE NOTICE 'NOT FOUND';
    RETURN NEW;
END;
$BODY$
  LANGUAGE plpgsql VOLATILE  COST 100;
ALTER FUNCTION event.queue_insert()  OWNER TO postgres;

CREATE TABLE event.queue
(
  event_id bigserial NOT NULL,
  server_id smallint NOT NULL,
  event character varying NOT NULL,
  param integer NOT NULL,
  version bigint NOT NULL DEFAULT nextval(('event.queue_version_seq'::text)::regclass),
  CONSTRAINT sync_idx PRIMARY KEY (server_id, event, param)
)
WITH (
  OIDS=FALSE
);
ALTER TABLE event.queue  OWNER TO postgres;

CREATE TRIGGER tr_event_queue
  BEFORE INSERT
  ON event.queue
  FOR EACH ROW
  EXECUTE PROCEDURE event.queue_insert();

CREATE OR REPLACE FUNCTION event.notify_table_upd()
  RETURNS trigger AS
$BODY$
DECLARE 
    key_name VARCHAR := NULL;
    relname varchar;
    param integer;
BEGIN

    relname := TG_TABLE_SCHEMA || '.' || TG_TABLE_NAME;
    --RAISE NOTICE '%', relname;
    
    EXECUTE 'SELECT key FROM event.syncparam WHERE name=' || quote_literal(relname) || ' AND enabled' INTO key_name;
    IF (key_name = NULL) THEN
        RETURN NULL;
    END IF;
    --RAISE NOTICE '%', key_name;
    
    IF ((TG_OP = 'INSERT') OR (TG_OP = 'UPDATE')) THEN
        EXECUTE format('SELECT ($1).%I::text', key_name) USING NEW INTO param;
    ELSE
        EXECUTE format('SELECT ($1).%I::text', key_name) USING OLD INTO param;
    END IF;
    
    --RAISE NOTICE '%', param;
    EXECUTE 'INSERT INTO event.queue(server_id, event, param, version) SELECT vpbx_id,' || quote_literal(relname) || ',' || param || ',0 FROM vpbx.settings';
    
    RETURN NULL;
END;
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
ALTER FUNCTION event.notify_table_upd()
  OWNER TO postgres;

CREATE TABLE event.syncparam
(
  name character varying,
  key character varying,
  enabled boolean
)
WITH (
  OIDS=FALSE
);
ALTER TABLE event.syncparam
  OWNER TO postgres;

