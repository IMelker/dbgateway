//
// Created by l2pic on 13.02.2021.
//

#ifndef DBGATEWAY__DBCONNECTION_H_
#define DBGATEWAY__DBCONNECTION_H_

#include <string>

class DBConnection
{
  public:
    struct SQLRequest {
        std::string method;
        std::string object;
    };

  public:
    DBConnection();
    ~DBConnection();

    bool request(SQLRequest&& request);

  private:

};

#endif //DBGATEWAY__DBCONNECTION_H_
