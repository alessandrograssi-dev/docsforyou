#include "Router.hpp"
#include <string>
#include <utility>

void Router::add_route(HttpMethod method, std::string path, Handler handler) {
  std::pair<HttpMethod, std::string> p(method, path);
  m_routes[p] = handler;
}

HttpResponse Router::route(const HttpRequest& req) const {
  std::pair<HttpMethod, std::string> p(req.method, req.target);
  
  auto it = m_routes.find(p);
  if (it == m_routes.end())
      return HttpResponse::notFound("Error: endpoint not found.\n");

  return it->second(req);
}