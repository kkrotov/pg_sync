#include "../classes/ConfBill.h"

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include "Exception.h"
#include "BDb.h"

bool ConfBill::parse_config_variables(boost::property_tree::ptree &pt) {

    web_port = pt.get<uint16_t>("main.web_port", 8032);
    db_main = pt.get<string>("db.master");
    db_calls = pt.get<string>("db.slave");

    BDb db_slave;
    db_slave.setCS(db_calls);
    string sql = "select value from vpbx.pg_server_settings where name='server_id'";
    BDbResult res = db_slave.query(sql);
    if (res.next()) {

        instance_id = res.get_i(0);
    }

//    db_bandwidth_limit_mbits = pt.get<double>("db.bandwidth_limit_mbits");
//    instance_id = pt.get<uint16_t>("geo.instance_id");
//    str_instance_id = boost::lexical_cast<string>(instance_id);
//    sql_regions_list = pt.get<string>("geo.sql_regions_list", "(" + str_instance_id + ")");
}
