#include "ThreadWeb.h"

#include "../web/http/server.hpp"
#include "../classes/AppBill.h"
#include "../web/HttpHandler.h"



void ThreadWeb::operator()() {
    if (app().conf.web_port == 0) return;

    http::server4::server(io_service, "0.0.0.0", string_fmt("%d", app().conf.web_port), HttpHandler())();
    io_service.run();
}

void ThreadWeb::stop() {
    io_service.stop();
}

