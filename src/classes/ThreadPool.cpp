#include "ThreadPool.h"
#include "Thread.h"

#include "../classes/App.h"

ThreadPool::ThreadPool(App * application) : app{application} {
}

void ThreadPool::run(Thread * thread) {

    thread->onStarted.connect(boost::bind(&ThreadPool::register_thread, this, _1));
    thread->onFinished.connect(boost::bind(&ThreadPool::unregister_thread, this, _1));
    thread->onRealStatusChanged.connect(boost::bind(&ThreadPool::thread_real_status_changed, this, _1));
    thread->status = getThreadStatusByAppStatus();
    thread->start();
}

void ThreadPool::register_thread(Thread *thread) {
    {
        lock_guard<std::mutex> lock(mutex);

        auto it = threads.begin();
        while (it != threads.end()) {
            Thread * tmp = *it;
            if (tmp->id == thread->id) {
                if (tmp == thread) {
                    return;
                }
                thread->id = thread->id + string_fmt("%d", rand());
                break;
            }
            ++it;
        }
        threads.push_back(thread);
    }

    thread_real_status_changed(0);
}

void ThreadPool::unregister_thread(Thread *thread) {

    {
        lock_guard<std::mutex> lock(mutex);

        auto it = threads.begin();
        while (it != threads.end()) {
            if (*it == thread) {
                thread->task_thread.detach();
                threads.erase(it);
                delete thread;
                break;
            }
            ++it;
        }
    }
    thread_real_status_changed(0);
}

bool ThreadPool::forAllThreads(std::function<bool(Thread*)> callback) {
    lock_guard<std::mutex> lock(mutex);

    for (auto it : threads) {
        if (!callback(it)) {
            return false;
        }
    }
    
    return true;
}

void ThreadPool::joinAll() {
    for (;;) {
        Thread * thread = 0;
        {
            lock_guard<std::mutex> lock(mutex);
            if (threads.begin() != threads.end()) {
                thread = * threads.begin();
            } else {
                break;
            }
        }
        thread->task_thread.join();
    }
}

void ThreadPool::thread_real_status_changed(Thread *thread) {

    AppStatus app_real_status = AppStatus::APP_STARTED;
    {
        lock_guard<std::mutex> lock(mutex);

        int count = 0;
        int initiated = 0;
        int preparing = 0;
        int running = 0;
        int paused = 0;
        int stopped = 0;

        auto it = threads.begin();
        while (it != threads.end()) {
            Thread *th = *it;
            ThreadStatus thread_real_status = th->getRealStatus();

            if (thread_real_status <= ThreadStatus::THREAD_WAITING) {
                initiated++;
            } else if (thread_real_status == ThreadStatus::THREAD_PREPARING) {
                preparing++;
            } else if (thread_real_status == ThreadStatus::THREAD_RUNNING) {
                running++;
            } else if (thread_real_status == ThreadStatus::THREAD_PAUSED) {
                paused++;
            } else if (thread_real_status == ThreadStatus::THREAD_STOPPED) {
                stopped++;
            }
            count++;
            ++it;
        }

        if (stopped == count) {
            app_real_status = AppStatus::APP_STOPPED;
        } else if (paused == count) {
            app_real_status = AppStatus::APP_PAUSED;
        } else if (running == count) {
            app_real_status = AppStatus::APP_RUNNING;
        } else if (initiated == count) {
            app_real_status = AppStatus::APP_INITIALIZING;
        } else {
            app_real_status = AppStatus::APP_PREPARING;
        }
    }
    
    app->setRealStatus(app_real_status);
}

void ThreadPool::app_status_changed() {

    lock_guard<std::mutex> lock(mutex);

    auto it = threads.begin();
    while (it != threads.end()) {
        Thread *thread = *it;
        thread->setStatus(getThreadStatusByAppStatus());
        ++it;
    }
}

void ThreadPool::app_real_status_changed() {

}

ThreadStatus ThreadPool::getThreadStatusByAppStatus() {
    AppStatus app_status = app->getStatus();

    if (app_status == AppStatus::APP_PREPARING) {
        return ThreadStatus::THREAD_PREPARING;
    } else if (app_status == AppStatus::APP_RUNNING) {
        return ThreadStatus::THREAD_RUNNING;
    } else if (app_status == AppStatus::APP_PAUSED) {
        return ThreadStatus::THREAD_PAUSED;
    } else if (app_status == AppStatus::APP_STOPPED) {
        return ThreadStatus::THREAD_STOPPED;
    } else {
        return ThreadStatus::THREAD_CREATED;
    }
}
