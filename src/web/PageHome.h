#pragma once

#include "BasePage.h"
#include "../classes/Thread.h"

class PageHome : public BasePage {

public:
    bool canHandle(std::string &path) {
        return path == "/";
    }
    void render(std::stringstream &html, map<string, string> &parameters) {

        renderHeader("",html);

        bool reset = parameters["action"]=="do_reset";
        html << "<table width=100%>\n";
        bool firstRow = true;
        time_t reset_time = 0;
        app().threads.forAllThreads([&](Thread* thread) {

                if (firstRow) {
                    thread->threadTotalsHeader(html);
                    firstRow = false;
                }
                if (reset) {

                    thread->resetErrors();
                }
                thread->threadTotalsData(html);
                if (thread->getResetTime()>0)
                    reset_time = thread->getResetTime();

                return true;
            });
        html << "</table>\n";

        string last_reset_time;
        if (reset_time>0)
            last_reset_time = "Last reset time: "+ string_time(reset_time);

        html << "<table border=0>\n";
        html << "<tr>";
        html << "<td style='text-align: left' nowrap>"+last_reset_time+"</td>";
        html << "<td style='text-align: left' nowrap>";
        html << "<br/>";
        html << "</tr>\n";
        html << "</table>\n";
        html << "<form action=\"\" method=\"POST\"> <button name=\"action\" value=\"do_reset\">Reset errors</button></form>";
    }
};