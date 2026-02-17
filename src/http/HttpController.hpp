#pragma once

#include "../services/CounterService.hpp"
#include "../services/DocumentService.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include <atomic>

class HttpController {
public:
  explicit HttpController(CounterService& counter_service, DocumentService& document_service);
  HttpResponse time(const HttpRequest& req);
  HttpResponse count(const HttpRequest& req);
  HttpResponse doc_get(const HttpRequest& req);
  HttpResponse doc_insert(const HttpRequest& req);
  HttpResponse doc_delete(const HttpRequest& req);

private:
  CounterService& m_counter_service;
  DocumentService& m_document_service;
};