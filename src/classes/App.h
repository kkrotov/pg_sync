#pragma once

#include "Conf.h"
#include "Logger.h"
#include "ThreadPool.h"

#include <boost/signals2.hpp>

enum AppStatus {
    APP_STARTED, APP_INITIALIZING, APP_PREPARING, APP_RUNNING, APP_PAUSED, APP_STOPPED
};

static const char * AppStatusNames[]{
    "STARTED", "INITIALIZING", "PREPARING", "RUNNING", "PAUSED", "STOPPED"
};

class App {
    time_t start_time;
public:
    Logger logger;
    ThreadPool threads;

    AppStatus status;
    AppStatus real_status;

    boost::signals2::signal<void() > onStatusChanged;
    boost::signals2::signal<void() > onRealStatusChanged;

    App();

    bool start();
    bool stop();
    bool run();

    void setStatus(AppStatus status);
    void setRealStatus(AppStatus real_status);
    AppStatus getStatus();
    AppStatus getRealStatus();
    time_t getStartTime() { return this->start_time; }

    double getRuntime();

protected:
    virtual void runApp() = 0;


};
