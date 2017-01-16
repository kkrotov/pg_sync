INSERT INTO vpbx.syncparam(name,  key, enabled) VALUES ('vpbx.announce', 'announce_id', true);
INSERT INTO vpbx.syncparam(name,  key, enabled) VALUES ('vpbx.asterisk_instance', 'asterisk_instance_id', true);

CREATE TRIGGER tr_announce
  AFTER INSERT OR UPDATE OR DELETE
  ON vpbx.announce
  FOR EACH ROW
  EXECUTE PROCEDURE event.notify_table_upd();

CREATE TRIGGER tr_asterisk_instance
  AFTER INSERT OR UPDATE OR DELETE
  ON vpbx.asterisk_instance
  FOR EACH ROW
  EXECUTE PROCEDURE event.notify_table_upd();

