#pragma once

#include "RestHandler.h"
#include "Channel.h"
#include "Gpio.h"

#include "../../common/IChannelAdapter.h"

#include <mutex>
#include <vector>

namespace homco2 {
namespace server {

class Server : public common::IChannelAdapter
{
public:
  Server();
  ~Server() = default;

  bool init();
  void run();

  common::ChannelState getState(common::ChannelId id) override;
  bool getMaster(common::ChannelId id) override;
  bool setMaster(common::ChannelId id, bool val) override;

  bool getOverride(common::ChannelId id) override;
  bool setOverride(common::ChannelId id, bool val) override;

  std::vector<common::WeekdayInterval> getIntervals(common::ChannelId id) override;
  bool setIntervals(common::ChannelId id, std::vector<common::WeekdayInterval> intervals) override;

private:
  RestHandler _handler;
  std::vector<Channel> _channels;
  Gpio _gpio;
  std::mutex _mutex;
};

}
}