#pragma once

#include "BasePull.h"

#include <map>
#include <vector>

#include "../classes/BDb.h"
#include "../classes/AppBill.h"

class BasePull;

class ManagerPull {

public:
    BDb db_master;
    BDb db_slave;
//    double bandwidth_limit_mbits;
    map<string, BasePull *> pulls;
    int part_size;
    int errors_count;
    vector<pair<string,string>> synclist;
    bool get_synclist();

    ManagerPull(string masterCS, string slaveCS);
    void add(BasePull * pull);
    void pull(int server_id);

    static ManagerPull * instance_master() {

        static ManagerPull inst(app().conf.db_main, app().conf.db_calls);
        return &inst;
    };
    static ManagerPull * instance_slave() {

        static ManagerPull inst(app().conf.db_calls, app().conf.db_main);
        return &inst;
    };

protected:
    void clearPulls();
    void runPulls();
};

