#include "Server.h"

#include <iostream>

int main()
{
  std::cout << "Starting server..." << std::endl;

  // Setup the rest api server.
  homco2::server::Server server;
  if (!server.init()) {
    std::cerr << "Could not initialize server!" << std::endl;
    return 1;
  }

  std::cout << "Server started" << std::endl;

  server.run();

  return 0;
}