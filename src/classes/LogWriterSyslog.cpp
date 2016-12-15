#include "LogWriterSyslog.h"
#include <syslog.h>

LogWriterSyslog::LogWriterSyslog(string ident, LogLevel minLevel, LogLevel maxLevel) : BaseLogWriter(minLevel, maxLevel) {
    this->ident = ident;
}

bool LogWriterSyslog::beforePublish() {

    openlog(ident.c_str(), LOG_PID, LOG_LOCAL0);
    return true;

}

void LogWriterSyslog::doPublish(pLogMessage message) {

    string msg = scount(message->count) + message->message;
    syslog(getSyslogLevel(message->level), "%s\n", msg.c_str());

}

void LogWriterSyslog::afterPublish() {

    closelog();

}

string LogWriterSyslog::logWriterName() {

    return string("LogWriterSyslog");
}