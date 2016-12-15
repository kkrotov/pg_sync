#pragma once

#include "BaseLogWriter.h"

class LogWriterFile : public BaseLogWriter {
public:
    LogWriterFile(string fileName, LogLevel minLevel = LogLevel::DEBUG, LogLevel maxLevel = LogLevel::CRITICAL);
protected:
    FILE* file;
    string fileName;
    bool beforePublish();
    void doPublish(pLogMessage message);
    void afterPublish();
    string logWriterName();
};


