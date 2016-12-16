#include "../classes/ConfBill.h"

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include "Exception.h"

bool ConfBill::parse_config_variables(boost::property_tree::ptree &pt) {

    web_port = pt.get<uint16_t>("main.web_port", 8032);

    db_main = pt.get<string>("db.main");
    db_calls = pt.get<string>("db.calls");
    db_bandwidth_limit_mbits = pt.get<double>("db.bandwidth_limit_mbits");

    instance_id = pt.get<uint16_t>("geo.instance_id");
    str_instance_id = boost::lexical_cast<string>(instance_id);
    sql_regions_list = pt.get<string>("geo.sql_regions_list", "(" + str_instance_id + ")");
}
