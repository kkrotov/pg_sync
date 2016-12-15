#include "../classes/ConfBill.h"

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include "Exception.h"

//void ConfBill::parse_biller_mode(string mode) {
//    string m = mode;
//    std::transform(m.begin(), m.end(), m.begin(), ::tolower);
//
//    if (m == "apihost") {
//        billerMode = APIHOST;
//    } else if (m == "multiregional") {
//        billerMode = MULTIREGIONAL;
//    } else if (m == "singleregional") {
//        billerMode = SINGLEREGIONAL;
//    } else
//        throw Exception("invalid biller mode:" + mode, "ConfBill::parse_biller_mode");
//}


bool ConfBill::parse_config_variables(boost::property_tree::ptree &pt) {

    web_port = pt.get<uint16_t>("main.web_port", 8032);

    db_main = pt.get<string>("db.main");
    db_calls = pt.get<string>("db.calls");
    db_bandwidth_limit_mbits = pt.get<double>("db.bandwidth_limit_mbits");

//    parse_biller_mode(pt.get<string>("main.mode", "SINGLEREGIONAL"));
//
//    string threads = pt.get<string>("main.run_threads", "");
//    if (threads.length() > 0) {
//
//        std::stringstream ss(threads);
//        string tmp;
//
//        while (ss >> tmp) {
//            active_threads.push_back(tmp);
//        }
//    }
//
//    threads = pt.get<string>("main.skip_threads", "");
//    if (threads.length() > 0) {
//
//        std::stringstream ss(threads);
//        string tmp;
//
//        while (ss >> tmp) {
//            skip_threads.push_back(tmp);
//        }
//    }
//    calls_raw_sync_delay = pt.get<int>("main.calls_raw_sync_delay", 5);
    instance_id = pt.get<uint16_t>("geo.instance_id");
    str_instance_id = boost::lexical_cast<string>(instance_id);
    sql_regions_list = pt.get<string>("geo.sql_regions_list", "(" + str_instance_id + ")");
//    hub_id = pt.get<uint16_t>("geo.hub_id", 0);
//
//    if (hub_id > 0 && billerMode != MULTIREGIONAL)
//        throw Exception("hub_id can only be used in multiregional mode", "ConfBill::parse_biller_mode");

//    openca_udp_host = pt.get<string>("udp.host", "");
//    openca_udp_port = pt.get<uint16_t>("udp.port", 0);
//    udp_openca_select_interval = pt.get<uint16_t>("udp.openca_select_interval", 10);
//    udp_force_finish_call_interval = pt.get<uint16_t>("udp.force_finish_call_interval", 5);
//
//    billing_free_seconds = pt.get<uint16_t>("billing.free_seconds", 5);
//
//    global_counters_select_interval = pt.get<uint16_t>("billing.global_counters_select_interval", 10);

//    radius_auth_port = pt.get<uint16_t>("radius.auth_port", 1812);
//    radius_acct_port = pt.get<uint16_t>("radius.acct_port", 1813);
//    radius_secret = pt.get<string>("radius.secret", "");
//
//    cdr_ftp_host = pt.get<string>("cdr_parcer.cdr_ftp_host", "");
//    cdr_ftp_user = pt.get<string>("cdr_parcer.cdr_ftp_user", "");
//    cdr_ftp_password = pt.get<string>("cdr_parcer.cdr_ftp_password", "");
//    cdr_ftp_dir = pt.get<string>("cdr_parcer.cdr_ftp_dir", "");
//    cdr_nasip = pt.get<string>("cdr_parcer.cdr_nasip", "127.0.0.1");
//    cdr_parcer_interval = pt.get<int>("cdr_parcer.interval", 1800);

//    call_sync_delay = get_int_vector(pt, "health.call_sync_delay");
//    cdr_sync_delay = get_int_vector(pt, "health.cdr_sync_delay");
//    call_save_delay = get_int_vector(pt, "health.call_save_delay");
//    cdr_proc_wait_count = get_int_vector(pt, "health.cdr_proc_wait_count");
//    call_save_wait_count = get_int_vector(pt, "health.call_save_wait_count");
//    thread_error_count = get_int_vector(pt, "health.thread_error_count");
//    trunk_max_load = get_int_vector(pt, "health.trunk_max_load");

    string source = pt.get<string>("sync.src");
    string dest = pt.get<string>("sync.dst");
    string key = pt.get<string>("sync.key");
    string event = pt.get<string>("sync.event");
    sync.push_back(SyncPar(source,dest,key,event));
}

//vector<int> ConfBill::get_int_vector(boost::property_tree::ptree &pt, string keyname) {
//
//    vector<int> v;
//    string keyval = pt.get<string>(keyname, "");
//    if (keyval.length() > 0) {
//
//        std::stringstream ss(keyval);
//        string tmp;
//        while (ss >> tmp)
//            v.push_back(std::stoi(tmp));
//    }
//    return v;
//}