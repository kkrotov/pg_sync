#pragma once

#include "../classes/Thread.h"

class ThreadLog : public Thread {
private:
    void run();

public:
    ThreadLog();
    static const char* idName() { return "log"; }
};

