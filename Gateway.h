//
// Created by imelker on 10.01.2021.
//

#ifndef DBGATEWAY__GATEWAY_H_
#define DBGATEWAY__GATEWAY_H_

#include <memory>
#include <thread>

#include "httplib.h"
using HttpServer = httplib::Server;
using HttpRequest = httplib::Request;
using HttpResponse =httplib::Response;

class Config;
class Logger;

class Gateway
{
  public:
    explicit Gateway(Config &config);
    ~Gateway();

  private:
    void initHttpControl();
    void initHttpSql();

    Config &config;
    std::unique_ptr<Logger> logger;
    std::unique_ptr<HttpServer> server;
    std::unique_ptr<Logger> serverLogger;
    std::thread serverThread;
};

#endif //DBGATEWAY__GATEWAY_H_
