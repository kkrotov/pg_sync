#pragma once

#include "../classes/Thread.h"
#include "../classes/BDb.h"
#include <list>

#include "../classes/Timer.h"
#include "../sync/ManagerPull.h"

class ThreadSyncMaster : public Thread {

    ManagerPull * manager;
    void run();

public:
    ThreadSyncMaster();
    static const char* idName() { return "sync_master"; }
};
