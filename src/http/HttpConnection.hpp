#pragma once

#include "boost/asio.hpp"
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include "Router.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

namespace net = boost::asio;            // from <boost/asio.hpp>
namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

class HttpConnection : public std::enable_shared_from_this<HttpConnection> {
public:
  HttpConnection(tcp::socket socket, Router& router)
        : m_socket(std::move(socket)), m_router(router) {
  }

  void start();

private:
  tcp::socket m_socket;
  Router& m_router;
  beast::flat_buffer m_buffer{8192};
  http::request<http::dynamic_body> m_request;
  http::response<http::dynamic_body> m_response;
  net::steady_timer m_deadline{
        m_socket.get_executor(), std::chrono::seconds(60)};

  void read_request();
  void process_request();
  void write_response();
  void check_deadline();

  HttpMethod to_HttpMethod(const http::verb& v) const;
};