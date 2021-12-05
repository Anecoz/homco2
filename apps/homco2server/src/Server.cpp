#include "Server.h"

#include "../common/WeekdayInterval.h"

#include <thread>

namespace homco2 {
namespace server {

Server::Server()
  : _handler(
      12345,
      {0, 1, 2, 3, 4, 5, 6, 7},
      this)
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

common::ChannelState Server::getState(common::ChannelId id)
{
  common::ChannelState answer;
  {
    std::lock_guard<std::mutex> lock(_mutex);
    answer = _channels[id].state();
  }
  return answer;
}

bool Server::getMaster(common::ChannelId id)
{
  bool answer = false;
  {
    std::lock_guard<std::mutex> lock(_mutex);
    answer = _channels[id].isMasterSet();
  }
  return answer;
}

bool Server::getOverride(common::ChannelId id)
{
  bool answer = false;
  {
    std::lock_guard<std::mutex> lock(_mutex);
    answer = _channels[id].isOverridden();
  }
  return answer;
}

bool Server::setOverride(common::ChannelId id, bool state)
{
  {
    std::lock_guard<std::mutex> lock(_mutex);
    _channels[id].override(state);
  }
  return true;
}

bool Server::setMaster(common::ChannelId id, bool state)
{
  {
    std::lock_guard<std::mutex> lock(_mutex);
    _channels[id].setMaster(state);
  }  
  return true;
}

bool Server::setIntervals(common::ChannelId id, std::vector<common::WeekdayInterval> intervals)
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

std::vector<common::WeekdayInterval> Server::getIntervals(common::ChannelId id)
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