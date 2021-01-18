//
// Created by imelker on 19.01.2021.
//

#ifndef DBGATEWAY_COMMON_HTTPUTILS_H_
#define DBGATEWAY_COMMON_HTTPUTILS_H_

#include "httplib.h"

using HttpHeaders = httplib::Headers;
using HttpRequest = httplib::Request;
using HttpResponse = httplib::Response;
using HttpMultipartFormDataMap = httplib::MultipartFormDataMap;

std::string dump_headers(const HttpHeaders &headers) {
    std::string s;
    char buf[BUFSIZ];

    for (const auto &[key, value] : headers) {
        snprintf(buf, sizeof(buf), "%s: %s\n", key.c_str(), value.c_str());
        s += buf;
    }

    return s;
}

std::string log(const HttpRequest &req, const HttpResponse &res) {
    std::string s;
    char buf[BUFSIZ];

    s += "================================\n";

    snprintf(buf, sizeof(buf), "%s %s %s", req.method.c_str(),
             req.version.c_str(), req.path.c_str());
    s += buf;

    std::string query;
    for (auto it = req.params.begin(); it != req.params.end(); ++it) {
        const auto &x = *it;
        snprintf(buf, sizeof(buf), "%c%s=%s",
                 (it == req.params.begin()) ? '?' : '&', x.first.c_str(),
                 x.second.c_str());
        query += buf;
    }
    snprintf(buf, sizeof(buf), "%s\n", query.c_str());
    s += buf;

    s += dump_headers(req.headers);

    s += "--------------------------------\n";

    snprintf(buf, sizeof(buf), "%d %s\n", res.status, res.version.c_str());
    s += buf;
    s += dump_headers(res.headers);
    s += "\n";

    if (!res.body.empty()) { s += res.body; }

    s += "\n";

    return s;
}

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

#endif //DBGATEWAY_COMMON_HTTPUTILS_H_
