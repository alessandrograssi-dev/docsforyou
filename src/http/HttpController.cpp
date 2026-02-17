#include "HttpController.hpp"
#include <ctime>
#include <iostream>
#include <nlohmann/json.hpp>
#include <regex>
#include <sstream>

using json = nlohmann::json;
#define CONTENT_JSON "application/json"
#define CONTENT_HTML "text/html"

HttpController::HttpController(CounterService& counter_service, DocumentService& document_service)
    : m_counter_service(counter_service), m_document_service(document_service)
{
}

HttpResponse HttpController::time(const HttpRequest& req)
{
  std::ostringstream os;
  os << "<html>\n"
     << "<head><title>Current time</title></head>\n"
     << "<body>\n"
     << "<h1>Current time</h1>\n"
     << "<p>The current time is " << std::time(0) << " seconds since the epoch.</p>\n"
     << "</body>\n"
     << "</html>\n";
  return HttpResponse::ok(os.str(), CONTENT_HTML);
}

HttpResponse HttpController::count(const HttpRequest& req)
{
  std::ostringstream os;
  os << "<html>\n"
     << "<head><title>Request count</title></head>\n"
     << "<body>\n"
     << "<h1>Request count</h1>\n"
     << "<p>There have been " << m_counter_service.increment() << " requests so far.</p>\n"
     << "</body>\n"
     << "</html>\n";
  return HttpResponse::ok(os.str(), CONTENT_HTML);
}

HttpResponse HttpController::doc_get(const HttpRequest& req)
{
  std::cout << "Getting doc " << req.target << '\n';
  try {
    std::string id = req.path_params.at("id");
    std::optional<Document> doc = m_document_service.get(id);
    if (doc.has_value()) {
      json j_body = {{"id", id}, {"author", doc.value().author}, {"content", doc.value().content}};
      return HttpResponse::ok(j_body.dump(), CONTENT_JSON);
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

    if (!j.contains("author") || !j.contains("content")) {
      return HttpResponse(400, "missing fields\n");
    }

    std::string author = j["author"];
    std::string content = j["content"];

    auto id = m_document_service.store(author, content);

    json response = {{"id", id}};
    return HttpResponse(201, response.dump(), CONTENT_JSON);
  }
  catch (...) {
    return HttpResponse(400, "Invalid JSON\n");
  }
}

HttpResponse HttpController::doc_delete(const HttpRequest& req)
{
  std::cout << "Deleting doc " << req.target << '\n';

  std::string id = req.path_params.at("id");
  if (m_document_service.remove(id)) {
    return HttpResponse::ok("Removed the file with ID " + id + '\n');
  }
  else {
    return HttpResponse::notFound("Document with ID '" + id + "' not found.\n");
  }
}