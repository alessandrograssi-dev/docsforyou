#pragma once
#include <cstdint>
#include <string>
#include <map>

struct HttpResponse {
    int status = 200;
    std::string content_type = "text/plain";
    std::string body;
    std::map<std::string, std::string> headers;

    static HttpResponse ok(std::string body) {
        return {200, "text/plain", std::move(body)};
    }

    static HttpResponse notFound(std::string message) {
        return {404, "text/plain", std::move(message)};
    }
};
