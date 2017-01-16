#pragma once

#include "Conf.h"
#include <map>

struct SyncPar {

    string src,
            dst,
            key,
            event;
    SyncPar (string src, string dst, string key, string event) {this->src=src;this->dst=dst;this->key=key, this->event=event;};
};

class ConfBill : public Conf {

public:

    string db_main;
    string db_calls;
    //double db_bandwidth_limit_mbits;
    uint16_t web_port;
//    vector<SyncPar> sync;

    uint16_t instance_id;
//    string str_instance_id;
//
//    string sql_regions_list;
//
//    string get_sql_regions_list() {
//        return sql_regions_list;
//    }

protected:
    virtual bool parse_config_variables(boost::property_tree::ptree &pt);
};


