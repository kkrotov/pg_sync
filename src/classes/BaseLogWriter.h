#pragma once

#include <list>
#include <memory>
#include <syslog.h>

#include "LogMessage.h"

using namespace std;

class BaseLogWriter {
public:
    BaseLogWriter(LogLevel minLevel, LogLevel maxLevel);
    virtual void massPublish(list<pLogMessage> messages);
    virtual string logWriterName() { return ""; };
protected:
    LogLevel minLevel;
    LogLevel maxLevel;
    virtual bool beforePublish();
    virtual void doPublish(pLogMessage message) {};
    virtual void afterPublish();
    string stime(time_t time);
    string slevel(LogLevel level);
    string scount(int count);
    int getSyslogLevel(LogLevel level);
};

typedef shared_ptr<BaseLogWriter> pLogWriter;