//
// Created by imelker on 10.01.2021.
//

#ifndef DBGATEWAY__GATEWAY_H_
#define DBGATEWAY__GATEWAY_H_

#include <thread>

#include "httplib.h"
using HttpServer = httplib::Server;

class Config;

class Gateway
{
  public:
    explicit Gateway(Config *config);
    ~Gateway();

  private:
    Config *config = nullptr;
    HttpServer server;
    std::thread serverThread;
};

#endif //DBGATEWAY__GATEWAY_H_
