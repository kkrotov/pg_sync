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

