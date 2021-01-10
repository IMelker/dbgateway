//
// Created by imelker on 10.01.2021.
//

#ifndef DBGATEWAY_COMMON_CONFIG_H_
#define DBGATEWAY_COMMON_CONFIG_H_

#include <string_view>
#include <toml.hpp>

class Config
{
  public:
    Config();
    explicit Config(const std::string& filepath);
    ~Config();

    template <typename KeyType, typename ValueType>
    auto insert(KeyType&& key, ValueType&& val) {
        return this->data.insert_or_assign(key, val);
    }

    template <typename KeyType>
    auto operator[](KeyType&& key) {
        return this->data[key];
    }

    void open(const std::string& filepath);
    void reopen();
    void save();
    void saveAs(const std::string& filepath);

    std::string dump();

  private:
    std::string path{};
    toml::table data{};
};

#endif //DBGATEWAY_COMMON_CONFIG_H_
