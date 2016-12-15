#pragma once

#include "BasePage.h"
#include "../classes/AppBill.h"

using namespace std;

class PageLog : public BasePage {
public:
    bool canHandle(std::string &path) {
        return path == "/log";
    }
    void render(std::stringstream &html, map<string, string> &parameters) {
        renderHeader("log",html);


        list<pLogMessage> history = app().logger.getHistory();

        html << "Queue length: <b>" << app().logger.getQueueLength() << "</b><br/>\n";
        html << "History length: <b>" << history.size() << "</b><br/>\n";
        html << "<br/>\n";

        html << "<table width=100%>\n";
        for (auto it = history.begin(); it != history.end(); ++it) {
            pLogMessage message = *it;
            html << "<tr>\n";
            html << "<td nowrap>" << message->countInGroup << "</td>\n";
            html << "<td nowrap>" << string_time(message->timeInGroup) << "</td>\n";
            html << "<td width=100%>" << message->message << "</td>\n";
            html << "</tr>\n";
        }
        html << "</table>\n";
    }
};