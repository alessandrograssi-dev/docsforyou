#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>

struct HttpResponse {
  int status = 200;
  std::string content_type = "text/plain";
  std::string body;
  std::unordered_map<std::string, std::string> headers;

  HttpResponse() = default;

  HttpResponse(int status, std::string body, std::string type = "text/plain")
      : status(status), content_type(std::move(type)), body(std::move(body))
  {
  }

  static HttpResponse ok(std::string body, std::string type = "text/plain")
  {
    return HttpResponse(200, body, type);
  }

  static HttpResponse notFound(std::string message)
  {
    return HttpResponse(404, message);
  }
};
