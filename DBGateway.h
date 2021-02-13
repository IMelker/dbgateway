//
// Created by imelker on 10.01.2021.
//

#ifndef DBGATEWAY__DBGATEWAY_H_
#define DBGATEWAY__DBGATEWAY_H_

#include <memory>
#include <thread>
#include <string>
#include <map>

#include "httplib.h"
using HttpServer = httplib::Server;
using HttpRequest = httplib::Request;
using HttpResponse =httplib::Response;

class Config;
class Logger;
class DBProcessor;

class DBGateway
{
  public:
    explicit DBGateway(Config &config);
    ~DBGateway();

  private:
    void initHttpControl();
    void initHttpSql();

    DBProcessor* getProcessor(std::string_view id);

    Config &config;
    std::unique_ptr<Logger> logger;

    std::mutex processorsMutex;
    std::map<std::string, std::unique_ptr<DBProcessor>, std::less<>> processors;

    std::unique_ptr<HttpServer> server;
    std::unique_ptr<Logger> serverLogger;
    std::thread serverThread;
};

#endif //DBGATEWAY__DBGATEWAY_H_
