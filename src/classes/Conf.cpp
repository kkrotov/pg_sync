#include "Conf.h"
#include <iostream>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <boost/property_tree/ini_parser.hpp>
#include <boost/lexical_cast.hpp>

bool Conf::readConfig(string config_file, string pid_file) {
    this->config_file = config_file;
    this->pid_file = pid_file;

    try {
        boost::property_tree::ptree pt;
        boost::property_tree::ini_parser::read_ini(config_file, pt);

        log_file_filename = pt.get<string>("log.file_filename", "");
        log_file_min_level = static_cast<LogLevel> (pt.get<uint8_t>("log.file_min_level", 0));
        log_file_max_level = static_cast<LogLevel> (pt.get<uint8_t>("log.file_max_level", 5));

        log_syslog_ident = pt.get<string>("log.syslog_ident", "");
        log_syslog_min_level = static_cast<LogLevel> (pt.get<uint8_t>("log.syslog_min_level", 0));
        log_syslog_max_level = static_cast<LogLevel> (pt.get<uint8_t>("log.syslog_max_level", 5));

        log_graylog_host = pt.get<string>("log.graylog_host", "");
        log_graylog_port = pt.get<uint16_t>("log.graylog_port", 12201);
        log_graylog_source = pt.get<string>("log.graylog_source", "dev");

        log_grouping_interval = pt.get<uint16_t>("log.grouping_interval", 60);

        parse_config_variables(pt);

    } catch (exception& e) {
        cout << "ERROR: Parse config file: " << e.what() << endl;
        return false;
    }
    return true;
}

#include <boost/algorithm/string.hpp>

