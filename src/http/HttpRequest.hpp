#pragma once
#include <string>
#include <string_view>
#include <unordered_map>

enum class HttpMethod {
  UNKNOWN,
  GET,
  POST,
  PUT,
  PATCH,
  DELETE,
};

struct HttpRequest {
  HttpMethod method = HttpMethod::UNKNOWN;
  std::string target;
  std::string body;
  std::unordered_map<std::string, std::string> path_params;
};

inline HttpMethod parse_HttpMethod(std::string_view method)
{
  if (method == "GET")
    return HttpMethod::GET;
  else if (method == "POST")
    return HttpMethod::POST;
  else if (method == "PUT")
    return HttpMethod::PUT;
  else if (method == "PATCH")
    return HttpMethod::PATCH;
  else if (method == "DELETE")
    return HttpMethod::DELETE;
  else
    return HttpMethod::UNKNOWN;
}
