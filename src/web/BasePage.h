#pragma once

#include <string>
#include <map>
#include <sstream>
#include "../classes/AppBill.h"
#include "../web/http/server.hpp"
#include "../web/http/reply.hpp"


using namespace std;

class BasePage {

    vector<pair<string,string>> header = {

            {"", "Home"},
//            {"calls", "Calls"},
//            {"trunks", "Trunks"},
//            {"data", "Data"},
//            {"data-billing", "Data billing"},
//            {"counters", "Counters"},
//            {"fmins", "Free minutes"},
//            {"packages", "Old Packages"},
//            {"nnp-package-minute-counters", "NNP Packages"},
//            {"trunk-settings-counters", "Minimalki"},
//            {"locks", "Locks"},
//            {"blacklist", "Blacklist"},
            {"sync", "Sync"},
//            {"cdrunfinished", "CDR Log"},
            {"log", "Log"},
//            {"config", "Config"},
//            {"health", "Health status"}
    };

public:
    virtual bool canHandle(std::string &path) = 0;
    virtual void render(std::stringstream &html, map<string, string> &parameters) = 0;
    void renderHeader(string active, std::stringstream &html);
};