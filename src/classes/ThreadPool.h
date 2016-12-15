#pragma once

#include <list>
#include <mutex>
using namespace std;

#include "Thread.h"
class App;

class ThreadPool {
public:
    ThreadPool(App * application);

    void run(Thread * thread);
    void joinAll();
    
    void app_status_changed();
    void app_real_status_changed();
    
    bool forAllThreads(std::function<bool(Thread*)> callback);
    bool isRegistered(std::string thread_id) {
        bool is_registered=false;
        forAllThreads([&](Thread* thread) {

            if (thread->id==thread_id)
                is_registered=true;

            return true;
        });
        return is_registered;
    };
    
private:
    std::mutex mutex;
    list<Thread *> threads;
    App * app;
    
    void register_thread(Thread * thread);
    void unregister_thread(Thread * thread);
    void thread_status_changed(Thread * thread);
    void thread_real_status_changed(Thread * thread);
    ThreadStatus getThreadStatusByAppStatus();

    bool shutdownFlag;
};
