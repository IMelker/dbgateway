//
// Created by imelker on 19.01.2021.
//

#ifndef DBGATEWAY_COMMON_HTTPUTILS_H_
#define DBGATEWAY_COMMON_HTTPUTILS_H_

#include "httplib.h"
#include "spdlog/fmt/ostr.h"
#include "fmt/format.h"

using HttpHeaders = httplib::Headers;
using HttpRequest = httplib::Request;
using HttpResponse = httplib::Response;
using HttpMultipartFormDataMap = httplib::MultipartFormDataMap;

std::string dump_multipart_files(const HttpMultipartFormDataMap &files) {
    std::string s;
    char buf[BUFSIZ];

    s += "--------------------------------\n";

    for (const auto &x : files) {
        const auto &name = x.first;
        const auto &file = x.second;

        snprintf(buf, sizeof(buf), "name: %s\n", name.c_str());
        s += buf;

        snprintf(buf, sizeof(buf), "filename: %s\n", file.filename.c_str());
        s += buf;

        snprintf(buf, sizeof(buf), "content type: %s\n", file.content_type.c_str());
        s += buf;

        snprintf(buf, sizeof(buf), "text length: %zu\n", file.content.size());
        s += buf;

        s += "----------------\n";
    }

    return s;
}

struct LogHttpQuery : public decltype(HttpRequest::params)
{
    template<typename OStream>
    friend OStream &operator<<(OStream &os, const LogHttpQuery &params) {
        std::string dump;
        for (auto &[key, value] : params)
            dump.append(fmt::format("{}{}={}", dump.empty() ? '?' : '&', key, value));
        return os << dump;
    }
};

struct LogHttpHeaders : public HttpHeaders
{
    template<typename OStream>
    friend OStream &operator<<(OStream &os, const LogHttpHeaders &headers) {
        std::string dump;
        for (auto &[key, value] : headers)
            dump.append(fmt::format("{}: {}\n", key, value));
        return os << dump;
    }
};

struct LogHttpRequest : public HttpRequest
{
    template<typename OStream>
    friend OStream &operator<<(OStream &os, const LogHttpRequest &req) {
        return os << fmt::format("{} {} {}{}\n{}\n", req.method, req.version, req.path, LogHttpQuery{req.params}, LogHttpHeaders{req.headers})
                  << (req.body.empty() ? "" : fmt::format("{}\n", req.body));
    }
};

struct LogHttpResponse : public HttpResponse
{
    template<typename OStream>
    friend OStream &operator<<(OStream &os, const LogHttpResponse &res) {
        return os << fmt::format("{} {}\n{}\n", res.status, res.version, LogHttpHeaders{res.headers})
                  << (res.body.empty() ? "" : fmt::format("{}\n", res.body));
    }
};

#endif //DBGATEWAY_COMMON_HTTPUTILS_H_
