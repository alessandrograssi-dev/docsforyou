#pragma once
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include <functional>
#include <regex>
#include <vector>

using Handler = std::function<HttpResponse(const HttpRequest&)>;

class Router {
public:
  using Handler = std::function<HttpResponse(HttpRequest)>;

  void add_route(HttpMethod method, const std::string& path_pattern, Handler handler);

  HttpResponse route(const HttpRequest& req);

private:
  struct RouteEntry {
    HttpMethod method;
    std::regex regex_pattern;
    std::vector<std::string> param_names;
    Handler handler;
  };

  std::vector<RouteEntry> m_routes;

  static std::regex build_regex(const std::string& pattern, std::vector<std::string>& param_names);
};
