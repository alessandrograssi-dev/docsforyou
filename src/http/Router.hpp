#pragma once
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include <functional>
#include <regex>
#include <vector>

using Handler = std::function<HttpResponse(const HttpRequest&)>;

class Router {
public:
  void add_route(HttpMethod method, std::regex path, Handler handler);
  HttpResponse route(const HttpRequest& req) const;

private:
  std::vector<std::pair<std::pair<HttpMethod, std::regex>, Handler>> m_routes;
};
