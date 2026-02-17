#include "HttpController.hpp"
#include <ctime>
#include <sstream>
#include <regex>
#include <iostream>

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

HttpResponse HttpController::doc_get(const HttpRequest& req) {
  std::cout << "Getting doc " << req.target << '\n';
  std::regex id_r = std::regex(R"(^/doc/(\d+)(/?)$)");
  std::smatch match;
  if (std::regex_match(req.target, match, id_r)){
    HttpResponse res;
    std::string id = match[1].str();
    std::optional<Document> doc = m_document_service.get(id);
    if (doc.has_value()) {
      res.status = 200;
      res.body += "ID:      " + id + '\n';
      res.body += "Author:  " + doc.value().author + '\n';
      res.body += "Content: \n" + doc.value().content + '\n';
    } else {
      res = HttpResponse::notFound("Document with ID '" + id + "' not found.\n");
    }
    return res;  
  }
  return HttpResponse::notFound("Invalid path.\n");
}

HttpResponse HttpController::doc_insert(const HttpRequest& req) {
  HttpResponse res;
  size_t author_index = req.body.find("author=");
  size_t content_index = req.body.find("content=");

  if (author_index != std::string::npos && content_index != std::string::npos) {
    size_t pos_separator = req.body.find("&");
    std::string author;
    std::string content;
    if (author_index < content_index) {
      author = req.body.substr(author_index + 7, pos_separator - 7);
      content = req.body.substr(content_index + 8, req.body.size() - pos_separator - 8);
    } else {
      content = req.body.substr(content_index + 8, pos_separator - 8);
      author = req.body.substr(author_index + 7, req.body.size() - pos_separator - 7);
    }
    std::string id = m_document_service.store(author, content);
    res.status = 201;
    res.body = "Inserted with ID: " + id + '\n';
  }
  else {
    res.status = 400;
    res.body = "Could not insert this record. Make sure you inserted author and content.\n";
  }
  return res;
}

HttpResponse HttpController::doc_delete(const HttpRequest& req) {
  std::cout << "Deleting doc " << req.target << '\n';
  HttpResponse res;
  std::regex id_r = std::regex(R"(^/doc/(\d+)(/?)$)");
  std::smatch match;
  std::regex_match(req.target, match, id_r);
  std::string id = match[1].str();
  
  if (m_document_service.remove(id)) {
    res.status = 200;
    res.body = "Removed the file with ID " + id + '\n';
  } else {
    res = HttpResponse::notFound("Document with ID '" + id + "' not found.\n");
  }
  return res;  
}