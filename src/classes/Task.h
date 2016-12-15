#pragma once

#include <string>

#include "../classes/BDb.h"
#include "../common.h"

#include "../classes/App.h"
#include "../classes/Spinlock.h"

using namespace std;

class Task {
protected:
    string server_id;
    BDb *db_main;
    string id;
    string params;
    string status;
    string name;
    Spinlock status_lock;
public:
    void setStatus(string str);
    string getStatus();
    string getParams();
    string getName();

    void initTask(BDb &db_main, string id, string params);

    virtual void html(stringstream &html);

    virtual void run() = 0;

    Task(string server_id);
};

