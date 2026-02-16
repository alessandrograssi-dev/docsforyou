#include "Server.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
  try {
    ServerOptions options(argc, argv);
    Server server(std::move(options));
    server.accept();
    server.run();
  }
  catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << '\n';
  }
}