#include "LogWriterScreen.h"

LogWriterScreen::LogWriterScreen(LogLevel minLevel, LogLevel maxLevel) : BaseLogWriter(minLevel, maxLevel) {
}

void LogWriterScreen::doPublish(pLogMessage message) {

    string msg = stime(message->log_time) + slevel(message->level) + scount(message->count) + message->message;
    printf("%s\n", msg.c_str());

}

string LogWriterScreen::logWriterName() {

    return string("LogWriterScreen");
}
