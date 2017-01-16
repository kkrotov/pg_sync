#include "../classes/AppBill.h"

#include "../threads/ThreadWeb.h"
#include "../threads/ThreadLog.h"
#include "../threads/ThreadSyncMaster.h"
#include "../threads/ThreadSyncSlave.h"

AppBill &app() {

    static AppBill appVar;
    return appVar;
}

void AppBill::runApp() {

    registerAllThreads();

    // Модуль веб интерфейса
    ThreadWeb web;
    std::thread web_thread(std::ref(web));

    runAppInSingleMode();
    threads.joinAll();

    web.stop();
    web_thread.join();
}

template<class T>
void AppBill::registerThread() {

    threadConstructorsMap[T::idName()] = []() -> Thread * { return new T(); };
}

Thread *AppBill::newThreadObject(std::string id) {

    auto fn = threadConstructorsMap[id];
    if (!fn)
        return 0;

    return fn();
}

void AppBill::registerAllThreads() {

    registerThread<ThreadLog>();
    registerThread<ThreadSyncMaster>();
    registerThread<ThreadSyncSlave>();
}

void AppBill::runAppInSingleMode() {

    std::vector<std::string> standardThreads{
            // Логирование
            "log",
            // Перемещение данных из таблиц центрального сервера на региональные
            "sync_master",
            // Перемещение данных из таблиц регионального сервера на центральные
            "sync_slave",
    };
    for (auto thread: standardThreads) {

        cout << "Running thread " + thread + "...\n";
        threads.run(newThreadObject(thread));
    }
}