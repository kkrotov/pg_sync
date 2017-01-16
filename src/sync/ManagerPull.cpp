#include "ManagerPull.h"
#include "../classes/AppBill.h"

ManagerPull::ManagerPull(string masterCS, string slaveCS) {

    part_size = 100;
    db_master.setCS(masterCS);
    db_slave.setCS(slaveCS);
    errors_count = 0;
    //bandwidth_limit_mbits = app().conf.db_bandwidth_limit_mbits;
}

void ManagerPull::add(BasePull * pull) {

    pull->setManager(this);
    pull->init();
    pulls.insert(make_pair(pull->event, pull));
}

bool ManagerPull::get_synclist() {

    string sql = "select name,key from event.syncparam where enabled";
    BDbResult res = db_master.query(sql);
    if (res.size() == 0)
        return false;

    while (res.next()) {

        string name = res.get_s(0);
        string key = res.get_s(1);
        synclist.push_back(make_pair(name,key));
    }
    return true;
}

//int ManagerPull::getInstanceId() {
//
//    int instance_id=0;
//    string sql = "select value from vpbx.pg_server_settings where name='server_id'";
//    BDbResult res = db_master.query(sql);
//    if (res.next()) {
//
//        instance_id = res.get_i(0);
//    }
//    return instance_id;
//}

void ManagerPull::pull(int server_id) {

    string select_events_query =
            "select event, param, version from event.queue where server_id=" + to_string(server_id) +
            " order by version limit " + lexical_cast<string>(part_size);

    while (true) {

        BDbResult res = db_master.query(select_events_query);
        if (res.size() == 0)
            break;

        clearPulls();

        while (res.next()) {

            string event = res.get_s(0);
            string id = res.get_s(1);

            map<string, BasePull *>::iterator it = pulls.find(event);
            if (it != pulls.end())
                it->second->addId(id);
            else
                db_master.exec("delete from event.queue where server_id=" + to_string(server_id) + " and event = '" + event + "'");
        }

        if (res.last()) {

            string version = res.get_s(2);
            for (auto it : pulls) {

                if (!it.second->need_pull)
                    continue;

                try {

                    db_master.exec("delete from event.queue where server_id=" + to_string(server_id) + " and event = '" + it.second->event + "' and version <= " + version);
                    it.second->pull();
                }
                catch (Exception &e) {

                    e.addTrace("ManagerPull:pull");
                    Log::exception(e);
                    errors_count += 1;
                }
            }
        }
    }
}

void ManagerPull::clearPulls() {

    for (auto it = pulls.cbegin(); it != pulls.cend(); ++it) {
        it->second->clear();
    }
}

void ManagerPull::runPulls() {

    for (auto it = pulls.cbegin(); it != pulls.cend(); ++it) {
        it->second->pull();
    }
}
