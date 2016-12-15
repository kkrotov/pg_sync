#pragma once


#include "BaseLogWriter.h"

class LogWriterScreen : public BaseLogWriter {
public:
    LogWriterScreen(LogLevel minLevel = LogLevel::DEBUG, LogLevel maxLevel = LogLevel::ERROR);
protected:
    void doPublish(pLogMessage message);
    string logWriterName();
};



