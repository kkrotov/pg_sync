#include "ThreadLog.h"
#include "../classes/AppBill.h"

void ThreadLog::run() {

    app().logger.processLogQueue();

}

ThreadLog::ThreadLog() {
    id = idName();
    name = "Log";
}


