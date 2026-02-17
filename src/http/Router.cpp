#include "Router.hpp"
#include <string>
#include <utility>

std::string to_string(const std::pair<HttpMethod, std::string>& e)
{
  std::string endp;
  if (e.first == HttpMethod::GET)
    endp = "< GET, ";
  if (e.first == HttpMethod::POST)
    endp = "< POST, ";
  endp += e.second + " >";
  return endp;
}

void Router::add_route(HttpMethod method, std::regex path, Handler handler)
{
  m_routes.push_back({{method, std::move(path)}, std::move(handler)});
}

HttpResponse Router::route(const HttpRequest& req) const
{
  auto it = m_routes.begin();
  for (; it != m_routes.end(); ++it) {
    if (it->first.first != req.method)
      continue;
    std::smatch match;
    if (std::regex_match(req.target, match, it->first.second))
      return it->second(req);
  }
  
  std::pair<HttpMethod, std::string> p(req.method, req.target);
  return HttpResponse::notFound("Error: endpoint" + to_string(p) + "not found.\n");
}