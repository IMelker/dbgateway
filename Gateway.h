//
// Created by imelker on 10.01.2021.
//

#ifndef DBGATEWAY__GATEWAY_H_
#define DBGATEWAY__GATEWAY_H_

#include <thread>

#include "httplib.h"
using HttpServer = httplib::Server;
using HttpRequest = httplib::Request;
using HttpResponse =httplib::Response;

class Config;

class Gateway
{
  public:
    explicit Gateway(Config &config);
    ~Gateway();

  private:
    void initHttpHandlers();

    Config &config;
    HttpServer *server;
    std::thread serverThread;
};

#endif //DBGATEWAY__GATEWAY_H_
