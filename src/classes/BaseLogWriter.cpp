#include "BaseLogWriter.h"
#include "../common.h"

BaseLogWriter::BaseLogWriter(LogLevel minLevel, LogLevel maxLevel) {

    this->minLevel = minLevel;
    this->maxLevel = maxLevel;

}

void BaseLogWriter::massPublish(list<pLogMessage> messages) {

    short publishStarted = -1;

    for (auto it = messages.begin(); it != messages.end(); ++it) {
        pLogMessage message = *it;

        if (message->level >= minLevel && message->level <= maxLevel) {

            if (publishStarted == -1) {
                publishStarted = (int) beforePublish();
            }

            if (publishStarted) {
                doPublish(message);
            }
        }

    }

    if (publishStarted == 1) {
        afterPublish();
    }

}

bool BaseLogWriter::beforePublish() {
    return true;
}

void BaseLogWriter::afterPublish() {
}

string BaseLogWriter::stime(time_t time) {
    return "[" + string_time(time, 1) + "] ";
}

string BaseLogWriter::slevel(LogLevel level) {
    switch (level) {
        case LogLevel::DEBUG:
            return "[DEBUG] ";
        case LogLevel::INFO:
            return "[INFO] ";
        case LogLevel::NOTICE:
            return "[NOTICE] ";
        case LogLevel::WARNING:
            return "[WARNING] ";
        case LogLevel::ERROR:
            return "[ERROR] ";
        case LogLevel::CRITICAL:
            return "[CRITICAL] ";
        default: return "";
    }
}

string BaseLogWriter::scount(int count) {
    if (count > 1) {
        return "[" + lexical_cast<string>(count) + "] ";
    } else {
        return "";
    }
}

int BaseLogWriter::getSyslogLevel(LogLevel level) {
    switch (level) {
        case LogLevel::DEBUG:
            return LOG_DEBUG;
        case LogLevel::INFO:
            return LOG_INFO;
        case LogLevel::NOTICE:
            return LOG_NOTICE;
        case LogLevel::WARNING:
            return LOG_WARNING;
        case LogLevel::ERROR:
            return LOG_ERR;
        case LogLevel::CRITICAL:
            return LOG_CRIT;
        default: return 0;
    }
}