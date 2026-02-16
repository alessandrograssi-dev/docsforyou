// Re-adapted from beast/example/http/server/small/http_server_small.cpp

#include "Server.hpp"

Server::Server(ServerOptions&& options) 
: m_options(std::move(options)),
  m_acceptor(tcp::acceptor(m_ioc, {m_options.address, m_options.port})), 
  m_socket(m_ioc) {
}

void Server::run () {
  m_ioc.run();
}


void Server::accept() {
  m_acceptor.async_accept(m_socket,
      [&](beast::error_code ec)
      {
          if(!ec)
              std::make_shared<HttpConnection>(std::move(m_socket))->start();
          accept();
      });
}