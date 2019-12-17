#include "Server.h"

#include "../common/WeekdayInterval.h"

namespace homco2 {
namespace server {

Server::Server()
  : _handler(
      12345,
      {0, 1, 2, 3, 4, 5, 6, 7},
      std::bind(&Server::channelStateCallback, this, std::placeholders::_1),
      std::bind(&Server::channelMasterStateCallback, this, std::placeholders::_1),
      std::bind(&Server::channelOverrideStateCallback, this, std::placeholders::_1),
      std::bind(&Server::channelOverrideCallback, this, std::placeholders::_1, std::placeholders::_2),
      std::bind(&Server::channelMasterCallback, this, std::placeholders::_1, std::placeholders::_2),
      std::bind(&Server::channelSetTimerCallback, this, std::placeholders::_1, std::placeholders::_2),
      std::bind(&Server::channelTimerStateCallback, this, std::placeholders::_1))
  , _gpio()
{
  for (unsigned i = 0; i < 8; ++i) {
    _channels.emplace_back(static_cast<common::ChannelId>(i), &_gpio);
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
    {
      std::lock_guard<std::mutex> lock(_mutex);
      for (auto& channel : _channels) {
        channel.update();
        if (channel.dirty()) {
          std::cout << "Channel " << std::to_string(channel.id()) << " is dirty, notifying" << std::endl;
          auto channelState = channel.state();
          _handler.notifySubscribers(channelState);
          channel.clean();
        }
      }
    }    

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }
}

bool Server::channelStateCallback(common::ChannelId id)
{
  bool answer = false;
  {
    std::lock_guard<std::mutex> lock(_mutex);
    answer = _channels[id].poll();
  }
  return answer;
}

bool Server::channelMasterStateCallback(common::ChannelId id)
{
  bool answer = false;
  {
    std::lock_guard<std::mutex> lock(_mutex);
    answer = _channels[id].isMasterSet();
  }
  return answer;
}

bool Server::channelOverrideStateCallback(common::ChannelId id)
{
  bool answer = false;
  {
    std::lock_guard<std::mutex> lock(_mutex);
    answer = _channels[id].isOverridden();
  }
  return answer;
}

bool Server::channelOverrideCallback(common::ChannelId id, bool state)
{
  {
    std::lock_guard<std::mutex> lock(_mutex);
    _channels[id].override(state);
  }
  return true;
}

bool Server::channelMasterCallback(common::ChannelId id, bool state)
{
  {
    std::lock_guard<std::mutex> lock(_mutex);
    _channels[id].setMaster(state);
  }  
  return true;
}

bool Server::channelSetTimerCallback(common::ChannelId id, std::vector<common::WeekdayInterval> intervals)
{
  for (auto& interval: intervals) {
    auto info = interval.info();
    std::cout << "Setting channel " + std::to_string(id) + ", info: " + info << std::endl;
  }

  {
    std::lock_guard<std::mutex> lock(_mutex);
    _channels[id].setIntervals(std::move(intervals));
  }

  return true;
}

std::vector<common::WeekdayInterval> Server::channelTimerStateCallback(common::ChannelId id)
{
  std::vector<common::WeekdayInterval> answer;
  {
    std::lock_guard<std::mutex> lock(_mutex);
    answer = _channels[id].intervals();
  }
  return answer;
}

}
}