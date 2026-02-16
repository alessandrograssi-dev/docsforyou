// Re-adapted from beast/example/http/server/small/http_server_small.cpp

#include "Server.hpp"
#include <sstream>

Server::Server(ServerOptions&& options) 
: m_options(std::move(options)),
  m_acceptor(tcp::acceptor(m_ioc, {m_options.address, m_options.port})), 
  m_socket(m_ioc) {
    m_router.add_route(HttpMethod::GET, "/count", 
      [] (const HttpRequest& req) {
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
          <<  ++sm_request_count
          <<  " requests so far.</p>\n"
          <<  "</body>\n"
          <<  "</html>\n";
        res.body = os.str();
        return res;
      }
    );

    m_router.add_route(HttpMethod::GET, "/time", 
      [] (const HttpRequest& req) {
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
    );
}

void Server::run () {
  m_ioc.run();
}


void Server::accept() {
  m_acceptor.async_accept(m_socket,
      [&](beast::error_code ec)
      {
          if(!ec)
              std::make_shared<HttpConnection>(std::move(m_socket), m_router)->start();
          accept();
      });
}