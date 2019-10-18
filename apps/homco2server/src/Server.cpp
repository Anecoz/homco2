#include "Server.h"

namespace homco2 {
namespace server {

Server::Server()
  : _handler(
    12345,
    {0, 1, 2, 3, 4, 5, 6, 7},
    std::bind(&Server::channelStateCallback, this, std::placeholders::_1),
    std::bind(&Server::channelSetCallback, this, std::placeholders::_1, std::placeholders::_2))    
  , _gpio()
{
  for (unsigned i = 0; i < 8; ++i) {
    _channels.emplace_back(static_cast<homco2::server::ChannelId>(i), &_gpio);
  }
}

bool Server::init()
{
  return _handler.init();
}

void Server::run()
{
  while (true) {
    // Update the channels.
    for (auto& channel : _channels) {
      channel.update();
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }
}

bool Server::channelStateCallback(ChannelId id)
{
  return _gpio.poll(id);
}

bool Server::channelSetCallback(ChannelId id, bool state)
{
  _gpio.set(id, state);
  return true;
}

}
}