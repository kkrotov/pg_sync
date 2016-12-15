#include "PullTable.h"

PullTable::PullTable (std::string source, std::string dest, std::string key, std::string event) {

    this->event = event;
    this->src_table = source;
    this->dst_table = dest;
    this->key = key;
}

void PullTable::init() {

    fields = get_field_names(manager->db_main, src_table);
    datatype = get_field_types(manager->db_calls, dst_table);
}

std::vector<std::string> PullTable::get_field_names(BDb db, string src) {

    vector<string> names;
    std::vector<pair<std::string,std::string>> nametypes = get_nametype(db, src);
    for (auto nametype : nametypes) {

        names.push_back(nametype.first);
    }
    return names;
}

std::vector<std::string> PullTable::get_field_types(BDb db, string dst) {

    vector<string> datatype;
    std::vector<pair<std::string,std::string>> nametypes = get_nametype(db, dst);
    for (auto nametype : nametypes) {

        datatype.push_back(nametype.first+" "+nametype.second);
    }
    return datatype;
}

std::vector<pair<std::string,std::string>> PullTable::get_nametype(BDb db, string dst) {

    string sql = "SELECT a.attnum, a.attname, format_type(a.atttypid, a.atttypmod) "
                         "FROM pg_attribute a "
                         "WHERE a.attnum > 0 "
                         "AND NOT a.attisdropped AND a.attrelid = '"+dst+"'::regclass "
                         "ORDER  BY a.attnum;";

    BDbResult res = db.query(sql);
    std::vector<pair<std::string,std::string>> nametype;
    while (res.next()) {

        std::string name = res.get_s(1);
        std::string type = res.get_s(2);
        nametype.push_back(make_pair(name,type));
    }
    return nametype;
}