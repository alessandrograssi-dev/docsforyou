#include "HttpController.hpp"
#include <sstream>
#include <ctime>

HttpController::HttpController(CounterService& counter_service, DocumentService& document_service) : m_counter_service(counter_service), m_document_service(document_service) {

}

HttpResponse HttpController::time(const HttpRequest& req) {
  HttpResponse res;
  res.status = 200;
  res.content_type = "text/html";
  std::ostringstream os;
  os
    <<  "<html>\n"
    <<  "<head><title>Current time</title></head>\n"
    <<  "<body>\n"
    <<  "<h1>Current time</h1>\n"
    <<  "<p>The current time is "
    <<  std::time(0)
    <<  " seconds since the epoch.</p>\n"
    <<  "</body>\n"
    <<  "</html>\n";
  res.body = os.str();
  return res;
}

HttpResponse HttpController::count(const HttpRequest& req) {
  HttpResponse res;
  res.status = 200;
  res.content_type = "text/html";
  std::ostringstream os;
  os
  << "<html>\n"
  <<  "<head><title>Request count</title></head>\n"
  <<  "<body>\n"
  <<  "<h1>Request count</h1>\n"
  <<  "<p>There have been "
  <<  m_counter_service.increment()
  <<  " requests so far.</p>\n"
  <<  "</body>\n"
  <<  "</html>\n";
  res.body = os.str();
  return res;
}

HttpResponse HttpController::doc(const HttpRequest& req) {  
  HttpResponse res;
  const std::map<std::string, std::string>& h = req.headers;
  switch (req.method) {
    case HttpMethod::GET:
      if (h.find("id") != h.end()) {
        std::optional<Document> doc = m_document_service.get(h.at("id"));
        if (doc.has_value()) {
          res.status = 200;
          res.body += "ID:      " + h.at("id") + '\n';
          res.body += "Author:  " + doc.value().author + '\n';
          res.body += "Content: \n" + doc.value().content + '\n';
        } else {
          res.status = 404;
          res.body = "Document with ID '" + h.at("id") + "' not found.\n";
        }
      } else {
        res.status = 400;
        res.body = "Missing 'id' header.\n";
      }
      break;
    case HttpMethod::POST:
      if (h.find("author") != h.end() && h.find("content") != h.end()) {
        std::string id = m_document_service.store(h.at("author"), h.at("content"));
        res.status = 202;
        res.body = "Inserted with ID: " + id + '\n';
      } else {
        res.status = 402;
        res.body = "Could not insert this record. Make sure you inserted author and content.\n";
      }
      break;

    case HttpMethod::DELETE:
      if (h.find("id") != h.end()) {
        if (m_document_service.remove(h.at("id"))) {
          res.status = 203;
          res.body = "Removed the file with ID " + h.at("id") +'\n';
        } else {
          res.status = 403;
          res.body = "Could not remove the file with ID " + h.at("id") + '\n';
        }
      } else {
        res.status = 400;
        res.body = "Missing 'id' header\n";
      }
      break;

    default:
      res.status = 404;
      res.body = "Erroneous /doc endpoint usage\n";
      break;
  }
  return res;
}