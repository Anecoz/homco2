#pragma once

#include "RestHandler.h"
#include "Channel.h"
#include "Gpio.h"

#include <mutex>
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
  common::ChannelState channelStateCallback(common::ChannelId id);
  bool channelMasterStateCallback(common::ChannelId id);
  bool channelOverrideStateCallback(common::ChannelId id);
  bool channelOverrideCallback(common::ChannelId id, bool state);
  bool channelMasterCallback(common::ChannelId id, bool state);
  bool channelSetTimerCallback(common::ChannelId id, std::vector<common::WeekdayInterval> intervals);
  std::vector<common::WeekdayInterval> channelTimerStateCallback(common::ChannelId id);

  RestHandler _handler;
  std::vector<Channel> _channels;
  Gpio _gpio;
  std::mutex _mutex;
};

}
}