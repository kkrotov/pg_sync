#pragma once

#include "ManagerPull.h"
#include "../classes/Timer.h"

#include <string>
#include <vector>
#include <set>

using namespace std;

class ManagerPull;
class BDb;

class BasePull {

public:
    bool need_pull;
    Timer timer;
    string event;
    int pull_count_full;
    int pull_count_partial;
    int pull_count_errors;

    virtual void init() = 0;
    bool get_relation_fields(BDb *db_slave);
    void pull();
    void clear();
    void addId(string &id);
    void setManager(ManagerPull * manager);

    BasePull();

protected:
    ManagerPull * manager;
    string src_table;
    string dst_table;
    string key;
    //string src_sql_where = "";
    
    vector<string> fields;
    vector<string> datatype;

    set<string> ids_to_pull;
    bool full_pull;

    void pull(BDb *db_master, BDb *db_slave);
    void pullFull(BDb *db_master, BDb *db_slave);
    void pullPartial(BDb *db_master, BDb *db_slave);

    string getQueryFields();
    string getFilterIds();
};
