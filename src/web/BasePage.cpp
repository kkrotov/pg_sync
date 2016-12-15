#include "BasePage.h"

void BasePage::renderHeader(string active, std::stringstream &html) {

    html << "<style>\n";
    html << "* {font-family:monospace !important;}\n";
    html << "td { text-align: center }\n";
    html << "mark { \n"
            "    background-color: #DCDCDC;\n"
            "    color: black;\n"
            "}\n";
    html << "</style>\n";

    for (auto h : header) {

        if (h.first==active)
            h.second = "<mark>" + h.second + "</mark>";

        html << " <a href='/"+h.first+"'>"+h.second+"</a> | ";
    }
    html << string_time(time(NULL)) << " ";

    AppStatus status = app().getStatus();
    AppStatus real_status = app().getRealStatus();
    html << "<b>" << AppStatusNames[real_status] << "</b>";
    if (status != real_status)
        html << " (" << AppStatusNames[status] << ")";

    html << "\n<hr/><br/>\n";
}