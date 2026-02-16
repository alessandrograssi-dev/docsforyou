#include "Router.hpp"
#include <string>
#include <utility>

std::string to_string(const std::pair<HttpMethod, std::string>& e) {
  std::string endp;
  if (e.first == HttpMethod::GET)
    endp = "< GET, ";
  if (e.first == HttpMethod::POST)
    endp = "< POST, ";
  endp += e.second + " >";
  return endp;
}

void Router::add_route(HttpMethod method, std::string path, Handler handler) {
  std::pair<HttpMethod, std::string> p(method, path);
  m_routes[p] = handler;
}

HttpResponse Router::route(const HttpRequest& req) const {
  std::pair<HttpMethod, std::string> p(req.method, req.target);
  
  auto it = m_routes.find(p);
  if (it == m_routes.end())
      return HttpResponse::notFound("Error: endpoint" + to_string(p) + "not found.\n");

  return it->second(req);
}