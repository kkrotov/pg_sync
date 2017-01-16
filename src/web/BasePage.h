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
            {"sync_master", "Master to slave"},
            {"sync_slave", "Slave to master"},
//            {"log", "Log"},
    };

public:
    virtual bool canHandle(std::string &path) = 0;
    virtual void render(std::stringstream &html, map<string, string> &parameters) = 0;
    void renderHeader(string active, std::stringstream &html);
};