#pragma once

#include "../classes/Thread.h"
#include "../classes/BDb.h"
#include <list>

#include "../classes/Timer.h"
#include "../sync/ManagerPull.h"

class ThreadSync : public Thread {

    ManagerPull * manager;
    void run();

public:
    ThreadSync();
    static const char* idName() { return "sync"; }
};
