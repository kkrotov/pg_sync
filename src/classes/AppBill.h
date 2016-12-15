#pragma once

#include "App.h"
#include "ConfBill.h"

class AppBill : public App {
public:
    ConfBill conf;

    Thread * newThreadObject(std::string id);

protected:
    void runApp();
    void runAppInSingleMode();
    void runActiveThreads();
    void setHealthCheck();

    void registerAllThreads();    
    template<class T> void registerThread();

    std::map<std::string, Thread * (*)()> threadConstructorsMap;
};

AppBill & app();


