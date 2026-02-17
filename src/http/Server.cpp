// Re-adapted from beast/example/http/server/small/http_server_small.cpp

#include "Server.hpp"
#include <regex>
#include <sstream>

#include "../utilities/utilities.hpp"

Server::Server(ServerOptions&& options)
    : m_options(std::move(options)),
      m_acceptor(tcp::acceptor(m_ioc, {m_options.address, m_options.port})), m_socket(m_ioc),
      m_counter_service(), m_document_repository(), m_document_service(m_document_repository),
      m_controller(m_counter_service, m_document_service)
{
  m_router.add_route(HttpMethod::GET, "/count",
                     [this](const HttpRequest& req) { return m_controller.count(req); });

  m_router.add_route(HttpMethod::GET, "/time",
                     [this](const HttpRequest& req) { return m_controller.time(req); });

  m_router.add_route(HttpMethod::GET, "/doc/{id}",
                     [this](const HttpRequest& req) { return m_controller.doc_get(req); });

  m_router.add_route(HttpMethod::POST, "/doc",
                     [this](const HttpRequest& req) { return m_controller.doc_insert(req); });

  m_router.add_route(HttpMethod::DELETE, "/doc/{id}",
                     [this](const HttpRequest& req) { return m_controller.doc_delete(req); });
}

void Server::run()
{
  m_ioc.run();
}

void Server::accept()
{
  m_acceptor.async_accept(m_socket, [&](beast::error_code ec) {
    if (!ec)
      std::make_shared<HttpConnection>(std::move(m_socket), m_router)->start();
    accept();
  });
}