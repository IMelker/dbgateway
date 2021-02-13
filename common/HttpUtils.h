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

enum HttpStatus {
    CONTINUE = 100,
    SWITCHING_PROTOCOL = 101,
    PROCESSING = 102,
    EARLY_HINTS = 103,
    OK = 200,
    CREATED = 201,
    ACCEPTED = 202,
    NON_AUTHORITATIVE_INFORMATION = 203,
    NO_CONTENT = 204,
    RESET_CONTENT = 205,
    PARTIAL_CONTENT = 206,
    MULTI_STATUS = 207,
    ALREADY_REPORTED = 208,
    IM_USED = 226,
    MULTIPLE_CHOICE = 300,
    MOVED_PERMANENTLY = 301,
    FOUND = 302,
    SEE_OTHER = 303,
    NOT_MODIFIED = 304,
    USE_PROXY = 305,
    UNUSED = 306,
    TEMPORARY_REDIRECT = 307,
    PERMANENT_REDIRECT = 308,
    BAD_REQUEST = 400,
    UNAUTHORIZED = 401,
    PAYMENT_REQUIRED = 402,
    FORBIDDEN = 403,
    NOT_FOUND = 404,
    METHOD_NOT_ALLOWED = 405,
    NOT_ACCEPTABLE = 406,
    PROXY_AUTHENTICATION_REQUIRED = 407,
    REQUEST_TIMEOUT = 408,
    CONFLICT = 409,
    GONE = 410,
    LENGTH_REQUIRED = 411,
    PRECONDITION_FAILED = 412,
    PAYLOAD_TOO_LARGE = 413,
    URI_TOO_LONG = 414,
    UNSUPPORTED_MEDIA_TYPE = 415,
    RANGE_NOT_SATISFIABLE = 416,
    EXPECTATION_FAILED = 417,
    IM_A_TEAPOT = 418,
    MISDIRECTED_REQUEST = 421,
    UNPROCESSABLE_ENTITY = 422,
    LOCKED = 423,
    FAILED_DEPENDENCY = 424,
    TOO_EARLY = 425,
    UPGRADE_REQUIRED = 426,
    PRECONDITION_REQUIRED = 428,
    TOO_MANY_REQUESTS = 429,
    REQUEST_HEADER_FIELDS_TOO_LARGE = 431,
    UNAVAILABLE_FOR_LEGAL_REASONS = 451,
    INTERNAL_SERVER_ERROR = 500,
    NOT_IMPLEMENTED = 501,
    BAD_GATEWAY = 502,
    SERVICE_UNAVAILABLE = 503,
    GATEWAY_TIMEOUT = 504,
    HTTP_VERSION_NOT_SUPPORTED = 505,
    VARIANT_ALSO_NEGOTIATES = 506,
    INSUFFICIENT_STORAGE = 507,
    LOOP_DETECTED = 508,
    NOT_EXTENDED = 510,
    NETWORK_AUTHENTICATION_REQUIRED = 511
};

inline const char *statusToString(int status) {
    switch (status) {
        case CONTINUE: return "Continue";
        case SWITCHING_PROTOCOL: return "Switching Protocol";
        case PROCESSING: return "Processing";
        case EARLY_HINTS: return "Early Hints";
        case OK: return "OK";
        case CREATED: return "Created";
        case ACCEPTED: return "Accepted";
        case NON_AUTHORITATIVE_INFORMATION: return "Non-Authoritative Information";
        case NO_CONTENT: return "No Content";
        case RESET_CONTENT: return "Reset Content";
        case PARTIAL_CONTENT: return "Partial Content";
        case MULTI_STATUS: return "Multi-Status";
        case ALREADY_REPORTED: return "Already Reported";
        case IM_USED: return "IM Used";
        case MULTIPLE_CHOICE: return "Multiple Choice";
        case MOVED_PERMANENTLY: return "Moved Permanently";
        case FOUND: return "Found";
        case SEE_OTHER: return "See Other";
        case NOT_MODIFIED: return "Not Modified";
        case USE_PROXY: return "Use Proxy";
        case UNUSED: return "unused";
        case TEMPORARY_REDIRECT: return "Temporary Redirect";
        case PERMANENT_REDIRECT: return "Permanent Redirect";
        case BAD_REQUEST: return "Bad Request";
        case UNAUTHORIZED: return "Unauthorized";
        case PAYMENT_REQUIRED: return "Payment Required";
        case FORBIDDEN: return "Forbidden";
        case NOT_FOUND: return "Not Found";
        case METHOD_NOT_ALLOWED: return "Method Not Allowed";
        case NOT_ACCEPTABLE: return "Not Acceptable";
        case PROXY_AUTHENTICATION_REQUIRED: return "Proxy Authentication Required";
        case REQUEST_TIMEOUT: return "Request Timeout";
        case CONFLICT: return "Conflict";
        case GONE: return "Gone";
        case LENGTH_REQUIRED: return "Length Required";
        case PRECONDITION_FAILED: return "Precondition Failed";
        case PAYLOAD_TOO_LARGE: return "Payload Too Large";
        case URI_TOO_LONG: return "URI Too Long";
        case UNSUPPORTED_MEDIA_TYPE: return "Unsupported Media Type";
        case RANGE_NOT_SATISFIABLE: return "Range Not Satisfiable";
        case EXPECTATION_FAILED: return "Expectation Failed";
        case IM_A_TEAPOT: return "I'm a teapot";
        case MISDIRECTED_REQUEST: return "Misdirected Request";
        case UNPROCESSABLE_ENTITY: return "Unprocessable Entity";
        case LOCKED: return "Locked";
        case FAILED_DEPENDENCY: return "Failed Dependency";
        case TOO_EARLY: return "Too Early";
        case UPGRADE_REQUIRED: return "Upgrade Required";
        case PRECONDITION_REQUIRED: return "Precondition Required";
        case TOO_MANY_REQUESTS: return "Too Many Requests";
        case REQUEST_HEADER_FIELDS_TOO_LARGE: return "Request Header Fields Too Large";
        case UNAVAILABLE_FOR_LEGAL_REASONS: return "Unavailable For Legal Reasons";
        case NOT_IMPLEMENTED: return "Not Implemented";
        case BAD_GATEWAY: return "Bad Gateway";
        case SERVICE_UNAVAILABLE: return "Service Unavailable";
        case GATEWAY_TIMEOUT: return "Gateway Timeout";
        case HTTP_VERSION_NOT_SUPPORTED: return "HTTP Version Not Supported";
        case VARIANT_ALSO_NEGOTIATES: return "Variant Also Negotiates";
        case INSUFFICIENT_STORAGE: return "Insufficient Storage";
        case LOOP_DETECTED: return "Loop Detected";
        case NOT_EXTENDED: return "Not Extended";
        case NETWORK_AUTHENTICATION_REQUIRED: return "Network Authentication Required";

        default:
        case INTERNAL_SERVER_ERROR: return "Internal Server Error";
    }
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
