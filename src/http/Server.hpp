#ifndef SERVER_H
#define SERVER_H

#include "HttpConnection.hpp"

struct ServerOptions {
  ServerOptions(int argc, char *argv[]) {
    if(argc != 3)
    {
      print_usage(argv[0]);
      throw std::invalid_argument("Invalid number of arguments");
    }
    address = net::ip::make_address(argv[1]);
    port = static_cast<unsigned short>(std::atoi(argv[2]));
  }

  ServerOptions(ServerOptions&& other) {
    server_name = std::move(other.server_name);
    address = std::move(other.address);
    port = other.port;

    other.server_name = "";
    other.address = boost::asio::ip::address();
    other.port = 0;
  }


  std::string server_name;
  boost::asio::ip::address address;
  unsigned short port;
private:
  void print_usage(const char* exe_name) {
    std::cerr << "Usage: " << exe_name << " <address> <port>\n";
    std::cerr << "  For IPv4, try:\n";
    std::cerr << "    receiver 0.0.0.0 80\n";
    std::cerr << "  For IPv6, try:\n";
    std::cerr << "    receiver 0::0 80\n";
  }
};





class Server {
public:
  Server(ServerOptions&& options);

  void run();
  void accept();
  void start();


private:
  ServerOptions m_options;
  size_t m_request_count{0};
  net::io_context m_ioc{1};
  tcp::acceptor m_acceptor;
  tcp::socket m_socket;
  
};





#endif