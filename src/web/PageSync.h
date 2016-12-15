#pragma once

#include "BasePage.h"
#include "../classes/AppBill.h"
#include "../sync/ManagerPull.h"

class PageSync : public BasePage {
public:
    bool canHandle(std::string &path) {
        return path == "/sync";
    }
    void render(std::stringstream &html, map<string, string> &parameters) {
        renderHeader("sync",html);

        auto manager = ManagerPull::instance();

        html << "Mode: <b>" << app().conf.getBillerMode() << "</b>&nbsp; \n";
        html << "Region: <b>" << app().conf.get_sql_regions_list() << "</b>&nbsp; \n";
        html << "Errors count: <b>" << manager->errors_count << "</b>\n";
        html << "<br/><br/>\n";

        html << "<table width=100% border=1>\n";
        html << "<tr><td></td><th>Full</th><th>Partial</th><th>Errors</th><th>Last time</th><th>Total time</th><tr>\n";
        for (auto it = manager->pulls.cbegin(); it != manager->pulls.cend(); ++it) {
            BasePull * pull = it->second;
            html << "<tr>" <<
            "<th>" << pull->event << "</th>" <<
            "<td>" << lexical_cast<string>(pull->pull_count_full) << "</td>" <<
            "<td>" << lexical_cast<string>(pull->pull_count_partial) << "</td>" <<
            "<td>" << lexical_cast<string>(pull->pull_count_errors) << "</td>" <<
            "<td>" << pull->timer.sloop() << "</td>" <<
            "<td>" << pull->timer.sfull() << "</td>" <<
            "</tr>\n";
        }
        html << "</table><br/>\n";
    }
};