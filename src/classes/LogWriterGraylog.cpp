#include "LogWriterGraylog.h"
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <json/json.h>
#include <boost/lexical_cast.hpp>

LogWriterGraylog::LogWriterGraylog(string graylogHost, uint16_t graylogPort, string graylogSource, string serverId) : BaseLogWriter(LogLevel::DEBUG, LogLevel::CRITICAL) {
    this->graylogHost = graylogHost;
    this->graylogPort = graylogPort;
    this->graylogSource = graylogSource;
    this->serverId = serverId;
}

void LogWriterGraylog::massPublish(list<pLogMessage> messages) {

    if (messages.size() == 0) {
        return;
    }

    boost::asio::io_service io_service;
    boost::asio::ip::tcp::socket socket(io_service);

    {
        boost::asio::ip::address addr = boost::asio::ip::address::from_string(graylogHost);
        boost::asio::ip::tcp::endpoint endpoint(addr, graylogPort);

        boost::system::error_code error = boost::asio::error::host_not_found;
        socket.connect(endpoint, error);
        if (error) {
            throw boost::system::system_error(error);
        }
    }

    {
        boost::asio::streambuf buffer;
        std::ostream stream_buffer(&buffer);

        for (auto message : messages) {

            message->params["source"] = graylogSource;
            message->params["server"] = serverId;
            message->params["timestamp"] = boost::lexical_cast<string>(message->log_time);
            message->params["message"] = scount(message->count) + message->message;
            message->params["level"] = getSyslogLevel(message->level);
            if (!message->type.empty()) {
                message->params["type"] = message->type;
            }

            stream_buffer << message->params;
            stream_buffer << '\0';
        }

        boost::system::error_code ignored_error;

        boost::asio::write(socket, buffer, boost::asio::transfer_all(), ignored_error);
    }

    socket.close();
}

string LogWriterGraylog::logWriterName() {

    return string("LogWriterGraylog");
}
