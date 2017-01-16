#pragma once

#include "../sync/ManagerPull.h"

class ThreadSyncSlave : public Thread {

    ManagerPull * manager;
    void run();

public:
    ThreadSyncSlave();
    static const char* idName() { return "sync_slave"; }
};