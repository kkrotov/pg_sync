#pragma once

#include "BasePull.h"

class PullTable : public BasePull {

    std::vector<std::string> get_field_names(BDb db, string src);
    std::vector<std::string> get_field_types(BDb db, string dst);
    std::vector<pair<std::string,std::string>> get_nametype(BDb db, string dst);
public:
    PullTable (std::string source, std::string dest, std::string key, std::string event);
    void init();
};