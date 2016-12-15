#include "App.h"

App::App() : threads{this} {
    onStatusChanged.connect(boost::bind(&ThreadPool::app_status_changed, &threads));
    onRealStatusChanged.connect(boost::bind(&ThreadPool::app_real_status_changed, &threads));

    status = AppStatus::APP_RUNNING;
    setRealStatus(AppStatus::APP_STARTED);
}

bool App::start() {

    setRealStatus(AppStatus::APP_INITIALIZING);

    start_time = time(NULL);
    runApp();
}

bool App::stop() {

}

bool App::run() {

    start();

    stop();

}

void App::setStatus(AppStatus status) {
    this->status = status;
    onStatusChanged();
}

void App::setRealStatus(AppStatus real_status) {
    this->real_status = real_status;
    onRealStatusChanged();
}

AppStatus App::getStatus() {
    return status;
}

AppStatus App::getRealStatus() {
    return real_status;
}

double App::getRuntime() {

    return difftime (time(NULL), getStartTime());
}