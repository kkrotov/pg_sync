#pragma once

#include "BaseLogWriter.h"

class LogWriterSyslog : public BaseLogWriter {
public:
    LogWriterSyslog(string ident, LogLevel minLevel = LogLevel::DEBUG, LogLevel maxLevel = LogLevel::CRITICAL);
protected:
    string ident;
    bool beforePublish();
    void doPublish(pLogMessage message);
    void afterPublish();
    string logWriterName();
};


