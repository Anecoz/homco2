#include "TCPServer.h"

#include <iostream>
#include <chrono>

int main()
{
  std::cout << "Starting server" << std::endl;
  homco2::server::TCPServer server;

  while (true) {
    server.purgeDeadConnections();

    auto packets = server.takePackets();
    if (!packets.empty()) {
      std::cout << "Server gave us " << std::to_string(packets.size()) << " new packets." << std::endl;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }

  return 0;
}