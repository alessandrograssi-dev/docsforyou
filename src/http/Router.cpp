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

void Router::add_route(HttpMethod method, const std::string& path_pattern, Handler handler)
{
  std::vector<std::string> param_names;
  std::regex reg = build_regex(path_pattern, param_names);

  m_routes.push_back({method, reg, param_names, handler});
}

HttpResponse Router::route(const HttpRequest& req)
{
  for (const auto& route : m_routes) {

    if (route.method != req.method)
      continue;

    std::smatch match;

    if (std::regex_match(req.target, match, route.regex_pattern)) {

      HttpRequest req_copy = req;

      for (size_t i = 0; i < route.param_names.size(); ++i)
        req_copy.path_params[route.param_names[i]] = match[i + 1];

      return route.handler(req_copy);
    }
  }

  return {404, "Not Found"};
}

std::regex Router::build_regex(const std::string& pattern, std::vector<std::string>& param_names)
{
  std::string regex_str = "^";
  std::regex param_regex(R"(\{([a-zA-Z0-9_]+)\})");

  std::sregex_iterator it(pattern.begin(), pattern.end(), param_regex);
  std::sregex_iterator end;

  size_t last_pos = 0;

  for (; it != end; ++it) {
    auto match = *it;

    regex_str += pattern.substr(last_pos, match.position() - last_pos);
    regex_str += "([^/]+)";

    param_names.push_back(match[1]);
    last_pos = match.position() + match.length();
  }

  regex_str += pattern.substr(last_pos);
  regex_str += "$";

  return std::regex(regex_str);
}