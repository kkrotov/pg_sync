#include "BasePull.h"

#include "../common.h"
#include "../classes/Exception.h"
#include "../classes/Log.h"

BasePull::BasePull() {
    pull_count_full = 0;
    pull_count_partial = 0;
    pull_count_errors = 0;
}

void BasePull::setManager(ManagerPull * manager) {
    this->manager = manager;
}

void BasePull::clear() {
    need_pull = false;
    full_pull = key == "";
    ids_to_pull.clear();
}

void BasePull::addId(string &id) {
    need_pull = true;
    if (!full_pull) {
        if (id == "0") {
            full_pull = true;
        } else {
            ids_to_pull.insert(id);
        }
    }
}

bool BasePull::get_relation_fields(BDb *db_slave) {

    fields.clear();
    datatype.clear();

    string sql = "SELECT a.attname, format_type(a.atttypid, a.atttypmod) "
                 "FROM pg_attribute a "
                 "WHERE a.attnum > 0 "
                 "AND NOT a.attisdropped AND a.attrelid = '"+src_table+"'::regclass "
                 "ORDER  BY a.attnum;";

    BDbResult res = db_slave->query(sql);
    while (res.next()) {

        std::string name = res.get_s(0);
        std::string type = res.get_s(1);
        fields.push_back(name);
        datatype.push_back(name+" "+type);
    }
    return true;
}

void BasePull::pull() {

    pull (&manager->db_master, &manager->db_slave);
}

void BasePull::pull(BDb *db_master, BDb *db_slave) {

    if (!get_relation_fields(db_slave))
        return;

//    if (!need_pull)
//        return;

    try {

        timer.start();
        if (full_pull) {
            pullFull(db_master, db_slave);
        } else {
            pullPartial(db_master, db_slave);
        }
        timer.stop();
    }
    catch (Exception &e) {

        timer.stop();
        e.addTrace("BasePull::pull: " + event);
        throw  e;
    }
}

void BasePull::pullFull(BDb *db_master, BDb *db_slave) {

    BDbTransaction trans(db_slave);

    string del = "delete from " + dst_table;
    db_slave->exec(del);

    string query_fields = getQueryFields();
    string field_types = join(datatype, ",");
    string sel = "select " + query_fields + " from " + src_table;
    BDb::copy_dblink(dst_table, query_fields, field_types, sel, db_master, db_slave);

    trans.commit();
    pull_count_full += 1;
}

void BasePull::pullPartial(BDb *db_master, BDb *db_slave) {

    BDbTransaction trans(db_slave);
    for (auto it : ids_to_pull) {

        string id_to_pull = it;
        BDbResult res = db_slave->query("select * from "+dst_table+" where "+key+"="+id_to_pull);
        if (res.size()>0) {

            BDbResult r = db_master->query("select * from "+dst_table+" where "+key+"="+id_to_pull);
            if (r.size()>0)
                BDb::update_dblink(dst_table, fields, datatype, key, id_to_pull, db_master, db_slave);
            else
                db_slave->exec("delete from "+dst_table+" where "+key+"="+id_to_pull);
        }
        else {

            string query_fields = getQueryFields();
            string sel = "select " + query_fields + " from " + src_table;
            if (datatype.size()>0) {

                sel += " where \"" + key + "\"=" + id_to_pull;
                string field_types = join(datatype, ",");
                BDb::copy_dblink(dst_table, query_fields, field_types, sel,  db_master, db_slave);
            }
        }
    }
    trans.commit();
    ids_to_pull.clear();
    pull_count_partial += 1;
}

#if 0
void BasePull::pullPartial(BDb *db_master, BDb *db_slave, double bandwidth_limit_mbits) {

    string query_fields = getQueryFields();

    string sel = "select " + query_fields + " from " + src_table;
    string del = "delete from " + dst_table + " where \"" + key + "\" in (" + getFilterIds() + ")";

    BDbTransaction trans(db_slave);
    db_slave->exec(del);

    if (datatype.size()>0) {

        sel += " where \"" + key + "\" in (" + getFilterIds() + ") ";
        string field_types = join(datatype, ",");
        BDb::copy_dblink(dst_table, query_fields, field_types, sel,  db_master, db_slave);
    }
    else {

        sel += " where \"" + key + "\" in (" + getFilterIds() + ") ";
        BDb::copy(dst_table, src_table, query_fields, sel, db_master, db_slave, bandwidth_limit_mbits);
    }
    trans.commit();

//    auto it = ids_to_pull.find(key);
//    if (it != ids_to_pull.end()) {
//        ids_to_pull.erase(it);
//    }
    ids_to_pull.clear();
    pull_count_partial += 1;

}
#endif

string BasePull::getQueryFields() {

    return "\"" + join(fields, "\",\"") + "\"";
}

string BasePull::getFilterIds() {

    return join(ids_to_pull, ",");
//    return "'" + join(ids_to_pull, "','") + "'";
}
