#pragma once

#include "../common.h"
#include "../classes/Exception.h"
#include "../classes/Timer.h"
#include "../classes/Log.h"
#include <thread>
#include <boost/signals2.hpp>
#include "../classes/ThreadInterrupted.h"

enum ThreadStatus {
    THREAD_CREATED, THREAD_WAITING, THREAD_PREPARING, THREAD_RUNNING, THREAD_PAUSED, THREAD_STOPPED
};

static const char * ThreadStatusNames[] = {
    "CREATED", "WAITING", "PREPARING", "RUNNING", "PAUSED", "STOPPED"
};

class Thread {
public:
    string id;
    // Инициализируется ThreadPool'ом - переделать: либо инициализировать через
    // аргумент в конструкторе, либо через setStatus().
    ThreadStatus status;

    boost::signals2::signal<void(Thread *) > onStarted;
    boost::signals2::signal<void(Thread *) > onFinished;
    boost::signals2::signal<void(Thread *) > onRealStatusChanged;

    std::thread task_thread;

    virtual ~Thread();

    void start(int exitAfterRunsCount = 0, bool skipPrepare = false);

    ThreadStatus getStatus();
    void setStatus(ThreadStatus status);
    
    ThreadStatus getRealStatus();

    void threadTotalsHeader(stringstream &html);
    void threadTotalsData(stringstream &html);

    void html(stringstream &html);

    virtual void htmlfull(stringstream &html);

    virtual bool hasFullHtml();

    int errorCount(string &errorMsg) {

        errorMsg = lastError;
        return this->errorsCount;
    };
    time_t getResetTime() { return errorResetTime; }

    void resetErrors() {

        errorsCount=0;
        errorResetTime = time(NULL);
        lastError="";
    };

protected:
    string name;

    int threadPrepareSleepSeconds;
    int threadSleepSeconds;
    Timer timer;
    int errorsCount = 0;
    string lastError;
    time_t errorResetTime=0;
    
    Thread();

    static void ssleep(unsigned int seconds);
    
private:
    ThreadStatus real_status;

    bool status_ready;
    bool status_prepared;
    
    int exitAfterRunsCount;
    int runsCount;

    static void usleep(unsigned int milliseconds);

    virtual bool ready();

    virtual bool prepare();

    virtual void run() = 0;
    
    void setRealStatus(ThreadStatus real_status);
    virtual void onShutdown() { }

    void operator()();
};

typedef shared_ptr<Thread> pThread;