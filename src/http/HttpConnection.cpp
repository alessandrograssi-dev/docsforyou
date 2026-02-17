#include "HttpConnection.hpp"

void HttpConnection::start()
{
  read_request();
  check_deadline();
}

void HttpConnection::read_request()
{
  auto self = shared_from_this();

  http::async_read(m_socket, m_buffer, m_request,
                   [self](beast::error_code ec, std::size_t bytes_transferred) {
                     boost::ignore_unused(bytes_transferred);
                     if (!ec)
                       self->process_request();
                   });
}

void HttpConnection::process_request()
{
  HttpRequest req;
  req.method = to_HttpMethod(m_request.method());
  req.body = beast::buffers_to_string(m_request.body().data());
  req.target = m_request.target();

  for (auto const& field : m_request.base()) {
    req.path_params.emplace(field.name_string(), field.value());
  }

  HttpResponse res = m_router.route(req);

  m_response.version(m_request.version());
  m_response.keep_alive(false);
  m_response.result(res.status);
  m_response.set(http::field::content_type, res.content_type);
  beast::ostream(m_response.body()) << res.body;
  m_response.content_length(m_response.body().size());

  write_response();
}

void HttpConnection::write_response()
{
  auto self = shared_from_this();

  http::async_write(m_socket, m_response, [self](beast::error_code ec, std::size_t) {
    self->m_socket.shutdown(tcp::socket::shutdown_send, ec);
    self->m_deadline.cancel();
  });
}
void HttpConnection::check_deadline()
{
  auto self = shared_from_this();

  m_deadline.async_wait([self](beast::error_code ec) {
    if (!ec) {
      // Close socket to cancel any outstanding operation.
      self->m_socket.close(ec);
    }
  });
}

HttpMethod HttpConnection::to_HttpMethod(const http::verb& v) const
{
  switch (v) {
  case http::verb::get:
    return HttpMethod::GET;

  case http::verb::post:
    return HttpMethod::POST;

  case http::verb::put:
    return HttpMethod::PUT;

  case http::verb::patch:
    return HttpMethod::PATCH;

  case http::verb::delete_:
    return HttpMethod::DELETE;

  default:
    return HttpMethod::UNKNOWN;
  }
}