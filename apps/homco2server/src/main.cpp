#include "TCPServer.h"
#include "Gpio.h"
#include "Channel.h"

#include <iostream>
#include <chrono>
#include <vector>

int main()
{
  std::cout << "Starting server" << std::endl;
  homco2::server::TCPServer server;

  // Gpio abstraction.
  homco2::server::Gpio gpio;

  // Setup the channels.
  std::vector<homco2::server::Channel> channels;
  for (unsigned i = 0; i < 8; ++i) {
    channels.emplace_back(static_cast<homco2::server::ChannelId>(i), &gpio);
  }

  while (true) {
    // Update the server.
    server.purgeDeadConnections();
    auto packets = server.takePackets();
    if (!packets.empty()) {
      std::cout << "Server gave us " << std::to_string(packets.size()) << " new packets." << std::endl;
      // TODO: Go through packets. Update channels based on packets.
    }

    // Update the channels.
    for (auto& channel: channels) {
      channel.update();
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }

  return 0;
}