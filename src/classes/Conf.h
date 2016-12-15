#pragma once

#include <string>
#include <vector>
using namespace std;

#include <boost/property_tree/ptree.hpp>

#include "BaseLogWriter.h"

class Conf {
public:
    string config_file;
    string pid_file;

    string log_file_filename;
    LogLevel log_file_min_level;
    LogLevel log_file_max_level;

    string log_syslog_ident;
    LogLevel log_syslog_min_level;
    LogLevel log_syslog_max_level;

    string log_graylog_host;
    uint16_t log_graylog_port;
    string log_graylog_source;

    uint16_t log_grouping_interval;

    bool readConfig(string config_file, string pid_file);
protected:
    virtual bool parse_config_variables(boost::property_tree::ptree &pt) = 0;

private:

};
