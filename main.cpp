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
    options.addOption<bool>("trace-exit-code", "t", "Trace exit code for daemon mode", "false");
    options.addOption<std::string>("pid-file-name", "p", "Filepath for process ID output", "/var/run/dbgateway.pid");
    options.parse(argc, argv);

#ifndef WIN32
    auto daemon = options.getValue<bool>("daemon");
    auto traceExitCode = options.getValue<bool>("trace-exit-code");
    auto pidFilename = options.getValue<std::string>("pid-file-name");
    if (daemon) {
        int r = fork();
        if (r == 0) {
            FILE *pidFile = fopen(pidFilename.c_str(), "w+");
            fprintf(pidFile, "%d", getpid());
            fclose(pidFile);
        } else {
            if (r > 0) {
                if (traceExitCode) {
                    int status;
                    waitpid(r, &status, 0);
                    printf("Exit code is %d\n", status);
                }
                return 0;
            } else
                puts("Can't start as a daemon\n");
        }
    }
#endif

    Config config{options.getValue<std::string>("config")};

    using namespace std::string_view_literals;
    std::string_view library_name = config["library"]["name"].value_or(""sv);
    std::string_view library_author = config["library"]["authors"][0].value_or(""sv);
    int64_t depends_on_cpp_version = config["dependencies"]["cpp"].value_or(0);

    std::cout << library_name << std::endl;
    std::cout << library_author << std::endl;
    std::cout << depends_on_cpp_version << std::endl;

    Gateway dbGateway(config);

    while (!SysSignal::serviceTerminated()) {
        std::this_thread::sleep_for(std::chrono::seconds{1});
    }
}
