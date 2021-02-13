//
// Created by imelker on 10.01.2021.
//

#include "DBGateway.h"

#include <memory>

#include "app.h"
#include "common/Config.h"
#include "common/HttpUtils.h"
#include "common/Logger.h"
#include "common/SysSignal.h"

#include "DBProcessor.h"

// <service> -> [http] -> [auth] -> [http queue] -> [db queue] -> [worker] -> [http queue] -> [http] -> <service>
//     1           N         1           1               1            N              1           N           1


DBGateway::DBGateway(Config &config) : config(config), server(new HttpServer()) {

    this->logger = std::make_unique<RotateLogger>("DBGateway", config["dbgateway"]["log_filepath"].value_or(""));
    this->logger->setLogLevel(fromString(config["dbgateway"]["log_level"].value_or("")));
    this->serverLogger = std::make_unique<RotateLogger>("HTTP", config["http"]["log_filepath"].value_or(""));
    this->serverLogger->setLogLevel(fromString(config["http"]["log_level"].value_or("")));

    this->processors["pg"] = std::make_unique<DBProcessor>();

    if (!this->server->is_valid()) {
        logger->logCritical("HTTP Server has an error...\n");
    }

    /*this->server->set_error_handler([](const HttpRequest & req, HttpResponse &res) {
        res.set_content(fmt::format("Error: {}", res.body), "text/html");
    });*/

    this->server->set_logger([logger = this->serverLogger.get()](const HttpRequest &req, const HttpResponse &res) {
        logger->logTrace("Request:\n{}", LogHttpRequest{req});
        logger->logTrace("Response:\n{}", LogHttpResponse{res});
        logger->flush();
    });

    initHttpControl();
    initHttpSql();

    this->serverThread = std::thread{[&] {
        this->server->listen(config["http"]["http_ip"].value_or("127.0.0.1"),
                             config["http"]["http_port"].value_or(8080));
    }};
}

DBGateway::~DBGateway() {
    this->server->stop();
    this->serverThread.join();
}

DBProcessor *DBGateway::getProcessor(std::string_view id) {
    std::lock_guard lg(this->processorsMutex);
    auto it = this->processors.find(id);
    return it == this->processors.end() ? nullptr : it->second.get();
}

void DBGateway::initHttpControl() {
    this->server->Get("/version", [](const HttpRequest & /*req*/, HttpResponse &res) {
        res.set_content(APP_NAME "." APP_VERSION " " APP_GIT_HASH " " APP_GIT_DATE, "text/plain");
    });

    this->server->Get("/loglevel", [](const HttpRequest & req, HttpResponse &res) {
        auto it = req.params.find("name");
        if (it == req.params.end()) {
            res.set_content("There is no logger name provided", "plain/text");
            res.status = BAD_REQUEST;
            return;
        }

        auto logger = spdlog::get(it->second);
        if (!logger) {
            res.set_content(fmt::format("There is no logger with such name: {}", it->second), "plain/text");
            res.status = NOT_FOUND;
            return;
        }

        it = req.params.find("level");
        if (it == req.params.end()) {
            res.set_content("There is no logger level provided", "plain/text");
            res.status = BAD_REQUEST;
            return;
        }

        auto level = static_cast<spdlog::level::level_enum>(fromString(it->second));

        logger->set_level(level);
        logger->flush();

        if (level == spdlog::level::level_enum::off) {
            res.set_content("Logger is turned off", "plain/text");
        } else {
            res.set_content(fmt::format("Logger level changed to {}", it->second), "plain/text");
        }
    });

    this->server->Get("/shutdown", [&](const HttpRequest & req, HttpResponse &res) {
        SysSignal::setServiceTerminated(true);
    });
}

void DBGateway::initHttpSql() {
    this->server->Post("/request/(.*)", [&](const HttpRequest &req, HttpResponse &res) {
        std::string id = req.matches[1];
        auto *processor = getProcessor(id);
        if (!processor) {
            res.set_content(fmt::format("Failed to find processor: {}", id), "plain/text");
            res.status = NOT_FOUND;
            return;
        }
        processor->process(req.body);
    });
}


