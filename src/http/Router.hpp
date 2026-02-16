#pragma once
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include <functional>
#include <map>

using Handler = std::function<HttpResponse(const HttpRequest&)>;

class Router {
public:
  void add_route(HttpMethod method, std::string path, Handler handler);
  HttpResponse route(const HttpRequest& req) const;

private:
  std::map<std::pair<HttpMethod, std::string>, Handler> m_routes;
};
