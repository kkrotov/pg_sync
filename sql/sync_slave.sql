INSERT INTO vpbx.syncparam(name,  key, enabled) VALUES ('vpbx.autolink_ip', 'id', true);

CREATE TRIGGER tr_autolink_ip
  AFTER INSERT OR UPDATE OR DELETE
  ON vpbx.autolink_ip
  FOR EACH ROW
  EXECUTE PROCEDURE event.notify_table_upd();
