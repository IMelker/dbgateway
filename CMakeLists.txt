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
message("-- App: ${APP_NAME} ${APP_VERSION}")

# build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
message("-- Build: ${CMAKE_BUILD_TYPE}")

# third-party libraries
message("[third-party]")
add_subdirectory(third-party)
include_directories(third-party/toml)
include_directories(third-party/opts/include)

configure_file(${CMAKE_SOURCE_DIR}/config/app.h.in ${CMAKE_BINARY_DIR}/app.h @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/config/config.toml.in ${CMAKE_BINARY_DIR}/config.toml @ONLY)
include_directories(${CMAKE_BINARY_DIR})

# executable configuration
message("[executable]")
add_executable(dbgateway
        main.cpp
        Gateway.cpp Gateway.h
        common/Config.cpp common/Config.h
        common/Options.cpp common/Options.h
        common/Logger.cpp common/Logger.h
        common/ThreadPool.cpp common/ThreadPool.h)
link_libraries(dbgateway)
