#ifndef HTTPCONNECTION_H
#define HTTPCONNECTION_H

#include "boost/asio.hpp"
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio.hpp>
#include <iostream>

namespace net = boost::asio;            // from <boost/asio.hpp>
namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

class HttpConnection : public std::enable_shared_from_this<HttpConnection> {
public:
  HttpConnection(tcp::socket socket)
        : m_socket(std::move(socket)) {
  }

  void start();

private:
  tcp::socket m_socket;
  beast::flat_buffer m_buffer{8192};
  http::request<http::dynamic_body> m_request;
  http::response<http::dynamic_body> m_response;
  net::steady_timer m_deadline{
        m_socket.get_executor(), std::chrono::seconds(60)};
  static inline size_t sm_request_count{0};

  void read_request();
  void process_request();
  void create_response();
  void write_response();
  void check_deadline();
};


#endif // HTTPCONNECTION_H