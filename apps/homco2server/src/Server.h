#pragma once

#include "RestHandler.h"
#include "Channel.h"
#include "Gpio.h"

#include <vector>

namespace homco2 {
namespace server {

class Server
{
public:
  Server();
  ~Server() = default;

  bool init();
  void run();

private:
  bool channelStateCallback(ChannelId id);
  bool channelSetCallback(ChannelId id, bool state);

  RestHandler _handler;
  std::vector<Channel> _channels;
  Gpio _gpio;
};

}
}