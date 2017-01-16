#include "ThreadSyncMaster.h"

#include "../sync/PullTable.h"
#include "../classes/AppBill.h"

void ThreadSyncMaster::run() {

    int instance_id = app().conf.instance_id;
    // master -> slave
    if (manager->synclist.size()>0)
        manager->pull(instance_id);
}

ThreadSyncMaster::ThreadSyncMaster() {

    id = idName();
    name = "SyncMaster";
    manager = ManagerPull::instance_master();

    // master -> slave
    if (manager->get_synclist()) {

        for (auto sync : manager->synclist) {

            string name = sync.first;
            string key = sync.second;
            manager->add(new PullTable(name, key));
        }
    }
}