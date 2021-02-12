//
// Created by imelker on 10.01.2021.
//

#include "Gateway.h"

#include <memory>

#include "app.h"
#include "common/Config.h"
#include "common/HttpUtils.h"
#include "common/logger/Logger.h"
#include "common/SysSignal.h"


// <service> -> [http] -> [auth] -> [http queue] -> [db queue] -> [result] -> [http queue] -> [http] -> <service>

Gateway::Gateway(Config &config) : config(config), server(new HttpServer()) {

    this->logger = std::make_unique<RotateLogger>("DBGateway", config["dbgateway"]["log_filepath"].value_or(""));
    this->logger->setLogLevel(fromString(config["dbgateway"]["log_level"].value_or("")));
    this->serverLogger = std::make_unique<RotateLogger>("HTTP", config["http"]["log_filepath"].value_or(""));
    this->serverLogger->setLogLevel(fromString(config["http"]["log_level"].value_or("")));

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

Gateway::~Gateway() {
    this->server->stop();
    this->serverThread.join();
}

void Gateway::initHttpControl() {
    this->server->Get("/version", [](const HttpRequest & /*req*/, HttpResponse &res) {
        res.set_content(APP_NAME "." APP_VERSION " " APP_GIT_HASH " " APP_GIT_DATE, "text/plain");
    });

    this->server->Get("/loglevel", [](const HttpRequest & req, HttpResponse &res) {
        auto it = req.params.find("name");
        if (it == req.params.end()) {
            res.set_content("There is no logger name provided", "plain/text");
            res.status = 400;
            return;
        }

        auto logger = spdlog::get(it->second);
        if (!logger) {
            res.set_content(fmt::format("There is no logger with such name: {}", it->second), "plain/text");
            res.status = 404;
            return;
        }

        it = req.params.find("level");
        if (it == req.params.end()) {
            res.set_content("There is no logger level provided", "plain/text");
            res.status = 400;
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


void Gateway::initHttpSql() {
    this->server->Post("/create/database", [](const HttpRequest &req, HttpResponse &res) {
        // CREATE DATABASE dbname;
    });

    this->server->Post("/create/table", [](const HttpRequest &req, HttpResponse &res) {
        //CREATE TABLE <table_name1> (
        //  <col_name1><col_type1>,
        //  <col_name2><col_type2>,
        //  <col_name3><col_type3>
        //  PRIMARY KEY(<col_name1>),
        //  FOREIGN KEY(<col_name2>) REFERENCES <table_name2>(<col_name2>)
        //);
    });

    this->server->Post("/create/view", [](const HttpRequest &req, HttpResponse &res) {
        //CREATE VIEW <view_name> AS
        //  SELECT <col_name1>, <col_name2>, …
        //  FROM <table_name>
        //  WHERE <condition>;
    });

    this->server->Post("/show/databases", [](const HttpRequest &req, HttpResponse &res) {
        //SHOW DATABASES
    });

    this->server->Post("/show/tables", [](const HttpRequest &req, HttpResponse &res) {
        //SHOW TABLES
    });

    this->server->Post("/drop/database", [](const HttpRequest &req, HttpResponse &res) {
        //DROP DATABASE <database_name>;
    });

    this->server->Post("/drop/table", [](const HttpRequest &req, HttpResponse &res) {
        //DROP TABLE <table_name>;
    });

    this->server->Post("/drop/view", [](const HttpRequest &req, HttpResponse &res) {
        //DROP VIEW <view_name>;
    });

    this->server->Post("/describe", [](const HttpRequest &req, HttpResponse &res) {
        //DESCRIBE <table_name>
    });

    this->server->Post("/insert", [](const HttpRequest &req, HttpResponse &res) {
        //INSERT INTO <table_name> (<col_name1>, <col_name2>, <col_name3>, …)
        //  VALUES (<value1>, <value2>, <value3>, …);
    });

    this->server->Post("/delete", [](const HttpRequest &req, HttpResponse &res) {
        //DELETE FROM <table_name>
    });

    this->server->Post("/select", [](const HttpRequest &req, HttpResponse &res) {
        //SELECT <col_name1>, <col_name2>, …
        //  FROM <table_name>;

        //SELECT COUNT(course_id), dept_name
        //  FROM course
        //  GROUP BY dept_name
        //  HAVING COUNT(course_id)>1;

        //SELECT prereq.course_id, title, dept_name, credits, prereq_id
        //  FROM prereq
        //  LEFT OUTER JOIN course
        //  ON prereq.course_id=course.course_id;

        //SELECT DISTINCT course_id
        //  FROM section
        //  WHERE semester = ‘Fall’ AND year= 2009 AND course_id IN (
        //    SELECT course_id
        //    FROM section
        //    WHERE semester = ‘Spring’ AND year= 2010
        //  );
    });

    this->server->Post("/select/distinct", [](const HttpRequest &req, HttpResponse &res) {
        //SELECT DISTINCT <col_name1>, <col_name2>, …
        //  FROM <table_name>;
    });

    this->server->Post("/update", [](const HttpRequest &req, HttpResponse &res) {
        //UPDATE <table_name>
        //  SET <col_name1> = <value1>, <col_name2> = <value2>, ...
        //  WHERE <condition>;
    });
}
