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

}