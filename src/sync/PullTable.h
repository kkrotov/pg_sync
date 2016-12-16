#pragma once

#include "BasePull.h"

class PullTable : public BasePull {

public:
    PullTable(std::string source, std::string key);
    PullTable (std::string source, std::string dest, std::string key, std::string event);
    void init();
};