//
// Created by l2pic on 13.02.2021.
//

#ifndef DBGATEWAY__DBPROCESSOR_H_
#define DBGATEWAY__DBPROCESSOR_H_

#include <memory>
#include <string_view>

class DBConnection;

class DBProcessor
{
    std::string_view empty;
  public:
    DBProcessor();
    ~DBProcessor();

    bool process(std::string_view body);

  private:
    std::unique_ptr<DBConnection> connections;
};

#endif //DBGATEWAY__DBPROCESSOR_H_
