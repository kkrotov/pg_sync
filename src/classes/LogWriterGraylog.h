#pragma once

#include "BaseLogWriter.h"

class LogWriterGraylog : public BaseLogWriter {
public:
    LogWriterGraylog(string graylogHost, uint16_t graylogPort, string graylogSource, string serverId);
protected:
    string graylogHost;
    uint16_t graylogPort;
    string graylogSource;
    string serverId;
    void massPublish(list<pLogMessage> messages);
    string logWriterName();
};


