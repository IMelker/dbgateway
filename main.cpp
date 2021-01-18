//
// Created by imelker on 10.01.2021.
//
#include "app.h"

#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#include "common/SysSignal.h"
#include "common/Config.h"
#include "common/Options.h"

#include "Gateway.h"

int main(int argc, const char* const* argv) {
    SysSignal::setupSignalHandling();

    Options options{APP_NAME};
    options.addOption<std::string>("config", "c", "Config filepath", "config.toml");
    options.addOption<bool>("daemon", "d", "Daemon mode", "false");
    options.parse(argc, argv);

    Config config{options.getValue<std::string>("config")};

    using namespace std::string_view_literals;
    std::string_view library_name = config["library"]["name"].value_or(""sv);
    std::string_view library_author = config["library"]["authors"][0].value_or(""sv);
    int64_t depends_on_cpp_version = config["dependencies"]["cpp"].value_or(0);

    std::cout << library_name << std::endl;
    std::cout << library_author << std::endl;
    std::cout << depends_on_cpp_version << std::endl;

    Gateway dbGateway(config);

    bool daemon = options.getValue<bool>("daemon");
    while (!SysSignal::serviceTerminated()) {
        std::this_thread::sleep_for(std::chrono::seconds{1});
    }
}
