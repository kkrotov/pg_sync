#pragma once

#include <queue>
#include <map>
#include <mutex>
using namespace std;

#include "BaseLogWriter.h"

class Logger {
protected:
    std::queue<pLogMessage> queue;
    std::map<string, pLogMessage> history;
    std::mutex mutex;
    list<pLogWriter> writers;
    unsigned short grouping_interval;
public:
    Logger();
    Logger(const Logger& orig);
    virtual ~Logger();

    void logMessage(pLogMessage message);
    void addLogWriter(pLogWriter writer);
    void setLogGroupingInterval(unsigned short grouping_interval);
    void processLogQueue();
    list<pLogMessage> getHistory();
    int getQueueLength();
private:
    time_t grouping_period;
    time_t getPeriod();
    void addMessageToHistory();
    void processGroupingMessages();
    void publishMessages(list<pLogMessage> &messages);
};
