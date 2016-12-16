#include "PullTable.h"
#include "../classes/Exception.h"
#include "../classes/Log.h"

PullTable::PullTable(std::string source, std::string key) {

    this->event = this->dst_table = this->src_table = source;
    this->key = key;
}

PullTable::PullTable(std::string source, std::string dest, std::string key, std::string event) {

    this->event = event;
    this->src_table = source;
    this->dst_table = dest;
    this->key = key;
}

void PullTable::init() {

//    event = "airp";
//    src_table = "auth.airp";
//    dst_table = "auth.airp";
//    key = "id";
//
//    fields.push_back("id");
//    fields.push_back("server_id");
//    fields.push_back("name");
//
//    datatype.push_back("id integer");
//    datatype.push_back("server_id integer");
//    datatype.push_back("name character varying(50)");

//    try {
//
//        get_nametype(manager->db_main, src_table);
//    }
//    catch (Exception &e) {
//
//        e.addTrace("ManagerPull:pull");
//        Log::exception(e);
//    }
//    for (auto n : nametype) {
//
//        fields.push_back(n.first);
//    }
//    for (auto n : nametype) {
//
//        datatype.push_back(n.first+" "+n.second);
//    }
}