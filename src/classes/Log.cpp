#include "Log.h"
#include "AppBill.h"

void Log::info(const string &text, time_t log_time) {

    pLogMessage message(new LogMessage);
    message->level = LogLevel::INFO;
    message->message = text;
    message->log_time = log_time;
    app().logger.logMessage(message);
}

void Log::info(pLogMessage message, time_t log_time) {

    message->level = LogLevel::INFO;
    message->log_time = log_time;
    app().logger.logMessage(message);
}

void Log::error(const string &text, time_t log_time) {

    pLogMessage message(new LogMessage);
    message->level = LogLevel::ERROR;
    message->message = text;
    message->log_time = log_time;
    app().logger.logMessage(message);
}

void Log::notice(const string &text, time_t log_time) {

    pLogMessage message(new LogMessage);
    message->level = LogLevel::NOTICE;
    message->message = text;
    message->log_time = log_time;
    app().logger.logMessage(message);
}

void Log::notice(pLogMessage message, time_t log_time) {

    message->level = LogLevel::NOTICE;
    message->log_time = log_time;
    app().logger.logMessage(message);
}

void Log::warning(const string &text, time_t log_time) {

    pLogMessage message(new LogMessage);
    message->level = LogLevel::WARNING;
    message->message = text;
    message->log_time = log_time;
    app().logger.logMessage(message);
}

void Log::debug(const string &text, time_t log_time) {

    pLogMessage message(new LogMessage);
    message->level = LogLevel::DEBUG;
    message->message = text;
    message->log_time = log_time;
    app().logger.logMessage(message);
}

void Log::exception(Exception &e) {

    Log::error(e.getFullMessage());

}

void Log::flush() {
    app().logger.processLogQueue();
}