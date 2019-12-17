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
  bool channelStateCallback(ChannelId id);
  bool channelMasterStateCallback(ChannelId id);
  bool channelOverrideStateCallback(ChannelId id);
  bool channelOverrideCallback(ChannelId id, bool state);
  bool channelMasterCallback(ChannelId id, bool state);
  bool channelSetTimerCallback(ChannelId id, std::vector<common::WeekdayInterval> intervals);
  std::vector<common::WeekdayInterval> channelTimerStateCallback(ChannelId id);

  RestHandler _handler;
  std::vector<Channel> _channels;
  Gpio _gpio;
  std::mutex _mutex;
};

}
}