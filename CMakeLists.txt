message("[main]")

cmake_minimum_required(VERSION 3.13)
project(dbgateway)

set(CMAKE_CXX_FLAGS "-Wpedantic -Wall -Wextra -pthread")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

set(CMAKE_CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)

# app info
set(APP_NAME "DBGateway")
set(APP_VERSION_MAJOR 0)
set(APP_VERSION_MINOR 0)
set(APP_VERSION_PATCH 1)
set(APP_VERSION ${APP_VERSION_MAJOR}.${APP_VERSION_MINOR}.${APP_VERSION_PATCH})
execute_process(COMMAND git rev-parse --verify HEAD OUTPUT_VARIABLE APP_GIT_HASH OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND git log -1 --pretty=format:%ad HEAD OUTPUT_VARIABLE APP_GIT_DATE OUTPUT_STRIP_TRAILING_WHITESPACE)
message("-- App: ${APP_NAME}.${APP_VERSION}.${APP_GIT_HASH} ${APP_GIT_DATE}")

# build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
message("-- Build: ${CMAKE_BUILD_TYPE}")

# third-party libraries
message("\n[third-party]")
add_subdirectory(third-party)
include_directories(third-party/spdlog/include)
include_directories(third-party/toml)
include_directories(third-party/opts/include)
include_directories(third-party/http)

configure_file(${CMAKE_SOURCE_DIR}/config/app.h.in ${CMAKE_BINARY_DIR}/app.h @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/config/config.toml.in ${CMAKE_BINARY_DIR}/config.toml @ONLY)
include_directories(${CMAKE_BINARY_DIR})

# executable configuration
message("\n[executable]")
add_executable(dbgateway
        main.cpp
        DBGateway.cpp DBGateway.h
        DBProcessor.cpp DBProcessor.h
        DBConnection.cpp DBConnection.h
        common/SysSignal.cpp common/SysSignal.h
        common/Config.cpp common/Config.h
        common/Options.cpp common/Options.h
        common/Logger.cpp common/Logger.h
        common/ThreadPool.cpp common/ThreadPool.h
        common/HttpUtils.h
        common/StringUtils.h)

target_link_libraries(dbgateway httplib spdlog absl::strings stdc++fs)
