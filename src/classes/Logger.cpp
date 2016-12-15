#include <map>
#include <iostream>
#include <sstream>

#include "Logger.h"

Logger::Logger() {
    grouping_period = 0;
}

Logger::~Logger() {
}

void Logger::logMessage(pLogMessage message) {
    lock_guard<std::mutex> lock(mutex);

    if (grouping_interval == 0) {

        if (message->log_time==0)
            message->log_time = time(NULL);

        message->timeInGroup = message->log_time;
        message->count = 1;
        message->countInGroup = 0;
        queue.push(message);
        return;
    }

    processGroupingMessages();

    pLogMessage history_message = history[message->message];
    if (history_message.get() == 0) {

        if (message->log_time==0)
            message->log_time = time(NULL);

        message->timeInGroup = getPeriod();
        message->count = 1;
        message->countInGroup = 0;
        history[message->message] = message;

        queue.push(message);
    } else {
        history_message->countInGroup++;
    }
}

void Logger::addLogWriter(pLogWriter writer) {
    writers.push_back(writer);
}

void Logger::setLogGroupingInterval(unsigned short grouping_interval) {
    this->grouping_interval = grouping_interval;
}

void Logger::processLogQueue() {

    list<pLogMessage> messages;

    {
        lock_guard<std::mutex> lock(mutex);

        if (grouping_interval > 0) {
            processGroupingMessages();
        }

        while (!queue.empty()) {
            messages.push_back(queue.front());
            queue.pop();
        }
    }
    publishMessages(messages);
}

void Logger::publishMessages(list<pLogMessage> &messages)
{
    std::stringstream errorstream;
    list<pLogWriter> good_writers;
    for (auto it = writers.begin(); it != writers.end(); ++it) {

        try {
            (*it)->massPublish(messages);
            good_writers.push_back(*it);
        }
        catch (std::exception& e) {

            errorstream << (*it)->logWriterName() << " ERROR processing log queue: " << e.what();
        }
        catch (...) {

            errorstream << (*it)->logWriterName() << ": ERROR processing log queue";
        }
    }
    if (!errorstream.str().empty()) {

        pLogMessage error(new LogMessage);
        error->level = LogLevel::ERROR;
        if (error->log_time==0)
            error->log_time = time(NULL);

        error->timeInGroup = error->log_time;
        error->count = 1;
        error->countInGroup = 0;
        error->message = errorstream.str();

        list<pLogMessage> messages;
        messages.push_back(error);
        for (auto it = good_writers.begin(); it != good_writers.end(); ++it) {

            try {
                (*it)->massPublish(messages);
            }
            catch (...) {

                std::cerr << (*it)->logWriterName() << ": ERROR processing log queue";
            }
        }
    }
}

void Logger::processGroupingMessages() {

    time_t period = getPeriod();
    if (period > grouping_period) {

        auto it = history.begin();
        while (it != history.end()) {
            pLogMessage message = it->second;

            if (message->countInGroup > 0) {
                message->log_time = message->timeInGroup;
                message->count = message->countInGroup;
                message->timeInGroup = period;
                message->countInGroup = 0;
                queue.push(message);
            }

            if (message->timeInGroup < grouping_period)
                history.erase(it++);
            else
                it++;

        }

        grouping_period = period;
    }
}

time_t Logger::getPeriod() {
    time_t t = time(NULL);
    return t - t % grouping_interval + grouping_interval;
}

list<pLogMessage> Logger::getHistory() {
    lock_guard<std::mutex> lock(mutex);

    list<pLogMessage> res;
    typename map<string, pLogMessage>::iterator i;
    typename map<string, pLogMessage>::iterator e;
    for (i = history.begin(), e = history.end(); i != e;) {
        res.push_back(i->second);
        i++;
    }

    return res;
}

int Logger::getQueueLength() {
    lock_guard<std::mutex> lock(mutex);
    return queue.size();
}