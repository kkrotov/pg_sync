#include "ThreadSync.h"

#include "../sync/PullTable.h"
#include "../classes/AppBill.h"

void ThreadSync::run() {

    manager->pull();
}

ThreadSync::ThreadSync() {

    id = idName();
    name = "Sync";
    manager = ManagerPull::instance();

    if (manager->get_synclist()) {

        for (auto sync : manager->synclist) {

            string name = sync.first;
            string key = sync.second;
            manager->add(new PullTable(name, key));
        }
    }
}