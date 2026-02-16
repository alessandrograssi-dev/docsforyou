#pragma once

#include <atomic>
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "../services/CounterService.hpp"
#include "../services/DocumentService.hpp"

class HttpController {
public:
  explicit HttpController(CounterService& counter_service, DocumentService& document_service);
  HttpResponse time(const HttpRequest& req);
  HttpResponse count(const HttpRequest& req);
  HttpResponse doc(const HttpRequest& req);

private:
  CounterService& m_counter_service;
  DocumentService& m_document_service;

};