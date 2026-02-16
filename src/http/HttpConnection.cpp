#include "HttpConnection.hpp"


void HttpConnection::start() {
    read_request();
    check_deadline();
}


void HttpConnection::read_request() {
  auto self = shared_from_this();

  http::async_read(
      m_socket,
      m_buffer,
      m_request,
      [self](beast::error_code ec,
          std::size_t bytes_transferred)
      {
          boost::ignore_unused(bytes_transferred);
          if(!ec)
              self->process_request();
      });
}

void HttpConnection::process_request() {
  m_response.version(m_request.version());
  m_response.keep_alive(false);

  switch(m_request.method())
  {
  case http::verb::get:
      m_response.result(http::status::ok);
      m_response.set(http::field::server, "Beast");
      create_response();
      break;

  default:
      // We return responses indicating an error if
      // we do not recognize the request method.
      m_response.result(http::status::bad_request);
      m_response.set(http::field::content_type, "text/plain");
      beast::ostream(m_response.body())
          << "Invalid request-method '"
          << std::string(m_request.method_string())
          << "'";
      break;
  }

  write_response();
}

void HttpConnection::create_response() {
  if(m_request.target() == "/count")
  {
      m_response.set(http::field::content_type, "text/html");
      beast::ostream(m_response.body())
          << "<html>\n"
          <<  "<head><title>Request count</title></head>\n"
          <<  "<body>\n"
          <<  "<h1>Request count</h1>\n"
          <<  "<p>There have been "
          <<  ++sm_request_count
          <<  " requests so far.</p>\n"
          <<  "</body>\n"
          <<  "</html>\n";
  }
  else if(m_request.target() == "/time")
  {
      m_response.set(http::field::content_type, "text/html");
      beast::ostream(m_response.body())
          <<  "<html>\n"
          <<  "<head><title>Current time</title></head>\n"
          <<  "<body>\n"
          <<  "<h1>Current time</h1>\n"
          <<  "<p>The current time is "
          <<  std::time(0)
          <<  " seconds since the epoch.</p>\n"
          <<  "</body>\n"
          <<  "</html>\n";
  }
  else
  {
      m_response.result(http::status::not_found);
      m_response.set(http::field::content_type, "text/plain");
      beast::ostream(m_response.body()) << "File not found\r\n";
  }
}

void HttpConnection::write_response() {
   auto self = shared_from_this();

  m_response.content_length(m_response.body().size());

  http::async_write(
      m_socket,
      m_response,
      [self](beast::error_code ec, std::size_t)
      {
          self->m_socket.shutdown(tcp::socket::shutdown_send, ec);
          self->m_deadline.cancel();
      });
}
void HttpConnection::check_deadline() {
   auto self = shared_from_this();

  m_deadline.async_wait(
      [self](beast::error_code ec)
      {
          if(!ec)
          {
              // Close socket to cancel any outstanding operation.
              self->m_socket.close(ec);
          }
      });
}