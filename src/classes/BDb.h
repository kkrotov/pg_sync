#pragma once

#include <libpq-fe.h>

#include <map>
#include <list>
#include <string>
#include <vector>

using namespace std;

#include "BDbResult.h"
#include "BDbTransaction.h"

class BDb {
protected:
    PGconn * conn;
    string connstr;
    int need_advisory_lock;
public:
    BDb();
    BDb(const string &connstr);
    ~BDb();

    void setCS(const string &connstr);
    string getCS() {return this->connstr;};

    PGconn * getConn();
    bool connect();

    bool connected();

    void disconnect();

    void needAdvisoryLock(int key);

    void exec(const char * squery);
    void exec(const string &squery);

    BDbResult query(const char * squery);
    BDbResult query(const string &squery);

    static void copy(string table_to, string table_from, string columns, string query, BDb *db_from, BDb *db_to, double bandwidth_limit_mbits = 0.0);
    static void copy_dblink(string dst_table, string fields, string columns, string query, BDb *db_from, BDb *db_to);
    static void update_dblink(string dst_table, vector<string> fields, vector<string> datatype, string key, string id_to_pull, BDb *db_from, BDb *db_to);

    bool ping();
    bool rel_exists (string rel);
    static bool test(const string &connstr);
};

