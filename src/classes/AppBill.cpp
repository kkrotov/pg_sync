#include "../classes/AppBill.h"

#include "../threads/ThreadWeb.h"
#include "../threads/ThreadLog.h"
#include "../threads/ThreadSync.h"

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
    registerThread<ThreadSync>();
}

void AppBill::runAppInSingleMode() {

    std::vector<std::string> standardThreads{
            // Логирование
            "log",
            // Перемещение данных из таблиц центрального сервера на региональные
            "sync",
    };
    for (auto thread: standardThreads) {

        if (std::find(conf.skip_threads.begin(), conf.skip_threads.end(), thread) != conf.skip_threads.end()) {

            cout << "Skipping thread " + thread + "...\n";
            continue;
        }
        cout << "Running thread " + thread + "...\n";
        threads.run(newThreadObject(thread));
    }
}

void AppBill::runActiveThreads() {
    for (auto thread_name: conf.active_threads) {

        Thread *t = newThreadObject(thread_name);
        if (t == nullptr) {

            cout << "ERROR: Unable to run thread " + thread_name + "\n";
            continue;
        }
        cout << "Running thread " + thread_name + "...\n";
        threads.run(t);
    }
}
