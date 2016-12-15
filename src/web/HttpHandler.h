#pragma once


#include "PageHome.h"
#include "PageSync.h"
#include "PageLog.h"
#include "http/request.hpp"
#include "http/reply.hpp"

class HttpHandler {
private:

    static void spawnHandlers(vector<shared_ptr<BasePage>> &handlers) {
        handlers.push_back(shared_ptr<BasePage>(new PageHome));
        handlers.push_back(shared_ptr<BasePage>(new PageSync));
        handlers.push_back(shared_ptr<BasePage>(new PageLog));
    }

public:

    void operator()(const http::server4::request& req, http::server4::reply& rep) {

        vector<shared_ptr<BasePage>> handlers;
        spawnHandlers(handlers);

        vector<string> uri;
        boost::algorithm::split(uri, req.uri, boost::algorithm::is_any_of("?"));

        map<string, string> parameters;
        if (uri.size() > 1) {
            string p;
            string v;
            vector<string> params;
            boost::algorithm::split(params, uri[1], boost::algorithm::is_any_of("&"));
            for (vector<string>::iterator i = params.begin(); i != params.end(); i++) {
                vector<string> pv;
                boost::algorithm::split(pv, *i, boost::algorithm::is_any_of("="));
                if (pv.size() == 2) {
                    url_decode(pv[0], p);
                    url_decode(pv[1], v);
                    parameters[p] = v;
                } else if (pv.size() == 2) {
                    url_decode(pv[0], p);
                    parameters[p] = "";
                }
            }
        }
        if (req.method=="POST" && req.content.size()>0) {

            string p;
            string v;
            vector<string> params;
            boost::algorithm::split(params, req.content, boost::algorithm::is_any_of("&"));
            for (vector<string>::iterator i = params.begin(); i != params.end(); i++) {
                vector<string> pv;
                boost::algorithm::split(pv, *i, boost::algorithm::is_any_of("="));
                if (pv.size() == 2) {
                    url_decode(pv[0], p);
                    url_decode(pv[1], v);
                    parameters[p] = v;
                } else if (pv.size() == 2) {
                    url_decode(pv[0], p);
                    parameters[p] = "";
                }
            }
        }

        // Decode url to path.
        std::string request_path;
        if (uri.size() == 0 || !url_decode(uri[0], request_path)) {
            rep = http::server4::reply::stock_reply(http::server4::reply::bad_request);
            return;
        }

        // Request path must be absolute and not contain "..".
        if (request_path.empty() || request_path[0] != '/'
            || request_path.find("..") != std::string::npos) {
            rep = http::server4::reply::stock_reply(http::server4::reply::bad_request);
            return;
        }

        stringstream html;

        for(auto handler : handlers) {
            if (handler->canHandle(request_path)) {
                handler->render(html, parameters);

                rep.status = http::server4::reply::ok;
                rep.content.append(html.str());
                rep.headers.resize(4);
                rep.headers[0].name = "Content-Type";
                rep.headers[0].value = "text/html; charset=utf-8";
                rep.headers[1].name = "Cache-Control";
                rep.headers[1].value = "no-store, no-cache, must-revalidate";
                rep.headers[2].name = "Expires";
                rep.headers[2].value = "Mon, 26 Jul 1997 05:00:00 GMT";
                rep.headers[3].name = "Content-Length";
                rep.headers[3].value = boost::lexical_cast<std::string>(rep.content.size());

                return;
            }
        }

        rep = http::server4::reply::stock_reply(http::server4::reply::not_found);
        return;
    }

private:
    static bool url_decode(const std::string& in, std::string& out) {
        out.clear();
        out.reserve(in.size());
        for (std::size_t i = 0; i < in.size(); ++i) {
            if (in[i] == '%') {
                if (i + 3 <= in.size()) {
                    int value = 0;
                    std::istringstream is(in.substr(i + 1, 2));
                    if (is >> std::hex >> value) {
                        out += static_cast<char> (value);
                        i += 2;
                    } else {
                        return false;
                    }
                } else {
                    return false;
                }
            } else if (in[i] == '+') {
                out += ' ';
            } else {
                out += in[i];
            }
        }
        return true;
    }
};
