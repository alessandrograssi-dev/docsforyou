#include "HttpController.hpp"
#include <ctime>
#include <iostream>
#include <nlohmann/json.hpp>
#include <regex>
#include <sstream>

using json = nlohmann::json;

HttpController::HttpController(CounterService& counter_service, DocumentService& document_service)
    : m_counter_service(counter_service), m_document_service(document_service)
{
}

HttpResponse HttpController::time(const HttpRequest& req)
{
  HttpResponse res;
  res.status = 200;
  res.content_type = "text/html";
  std::ostringstream os;
  os << "<html>\n"
     << "<head><title>Current time</title></head>\n"
     << "<body>\n"
     << "<h1>Current time</h1>\n"
     << "<p>The current time is " << std::time(0) << " seconds since the epoch.</p>\n"
     << "</body>\n"
     << "</html>\n";
  res.body = os.str();
  return res;
}

HttpResponse HttpController::count(const HttpRequest& req)
{
  HttpResponse res;
  res.status = 200;
  res.content_type = "text/html";
  std::ostringstream os;
  os << "<html>\n"
     << "<head><title>Request count</title></head>\n"
     << "<body>\n"
     << "<h1>Request count</h1>\n"
     << "<p>There have been " << m_counter_service.increment() << " requests so far.</p>\n"
     << "</body>\n"
     << "</html>\n";
  res.body = os.str();
  return res;
}

HttpResponse HttpController::doc_get(const HttpRequest& req)
{
  std::cout << "Getting doc " << req.target << '\n';
  try {
    std::string id = req.path_params.at("id");
    std::optional<Document> doc = m_document_service.get(id);
    if (doc.has_value()) {
      json j_body = {{"id", id},
                     {"info", {{"author", doc.value().author}, {"content", doc.value().content}}}};
      HttpResponse res;
      res.status = 200;
      res.body = j_body.dump();
      return res;
    }
    else {
      return HttpResponse::notFound("Document with ID '" + id + "' not found.\n");
    }
  }
  catch (...) {
    return HttpResponse::notFound("Invalid path.\n");
  }
}

HttpResponse HttpController::doc_insert(const HttpRequest& req)
{
  try {
    auto j = json::parse(req.body);

    if (!j.contains("author") || !j.contains("content"))
      return {400, "Missing fields"};

    std::string author = j["author"];
    std::string content = j["content"];

    auto id = m_document_service.store(author, content);

    json response = {{"id", id}};
    HttpResponse res;
    res.status = 201;
    res.body = response.dump();
    res.content_type = "application/json";
    return res;
  }
  catch (...) {
    return {400, "Invalid JSON"};
  }
}

HttpResponse HttpController::doc_delete(const HttpRequest& req)
{
  std::cout << "Deleting doc " << req.target << '\n';
  HttpResponse res;

  std::string id = req.path_params.at("id");
  if (m_document_service.remove(id)) {
    res.status = 200;
    res.body = "Removed the file with ID " + id + '\n';
  }
  else {
    res = HttpResponse::notFound("Document with ID '" + id + "' not found.\n");
  }
  return res;
}