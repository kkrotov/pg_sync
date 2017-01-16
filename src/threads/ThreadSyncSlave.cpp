#include "ThreadSyncSlave.h"
#include "../sync/PullTable.h"

void ThreadSyncSlave::run() {

    int instance_id = app().conf.instance_id;
    // slave -> master
    if (manager->synclist.size()>0)
        manager->pull(instance_id);
}

ThreadSyncSlave::ThreadSyncSlave() {

    id = idName();
    name = "SyncSlave";
    manager = ManagerPull::instance_slave();

    // slave -> master
    if (manager->get_synclist()) {

        for (auto sync : manager->synclist) {

            string name = sync.first;
            string key = sync.second;
            manager->add(new PullTable(name, key));
        }
    }
}