#pragma once

#include <string>
#include <memory>
#include <map>
#include <json/json.h>

enum LogLevel {
    DEBUG, INFO, NOTICE, WARNING, ERROR, CRITICAL
};

struct LogMessage {
    LogLevel level;
    std::string message;
    std::string type;
    time_t log_time;
    time_t timeInGroup;
    int count;
    int countInGroup;
    Json::Value params;
};

typedef std::shared_ptr<LogMessage> pLogMessage;