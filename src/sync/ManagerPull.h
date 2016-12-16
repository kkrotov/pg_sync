#pragma once

#include "BasePull.h"

#include <map>
#include <vector>

#include "../classes/BDb.h"

class BasePull;

class ManagerPull {
public:
    BDb db_main;
    BDb db_calls;
    double bandwidth_limit_mbits;
    map<string, BasePull *> pulls;
    int part_size;
    int errors_count;
    vector<pair<string,string>> synclist;
    bool get_synclist();

    ManagerPull();
    void add(BasePull * pull);
    void pull();

    static ManagerPull * instance() {
        static ManagerPull inst;
        return &inst;
    };

protected:
    void clearPulls();
    void runPulls();
};

