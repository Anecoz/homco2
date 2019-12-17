#include "Channel.h"
#include "Gpio.h"

namespace homco2 {
namespace server {

Channel::Channel(ChannelId channelId, Gpio* gpio)
  : _channelId(channelId)
  , _gpio(gpio)
  , _master(true)
  , _overridden(false)
  , _state(false)
  , _runOnce(false)  
{
  _state = _gpio->poll(_channelId);
}

Channel::~Channel()
{}

void Channel::setIntervals(std::vector<common::WeekdayInterval>&& intervals)
{
  _intervals = std::move(intervals);
}

std::vector<common::WeekdayInterval> Channel::intervals()
{
  return _intervals;
}

void Channel::setState(bool state)
{
  _gpio->set(_channelId, state);
  _state = state;
}

bool Channel::poll()
{
  return _gpio->poll(_channelId);
}

void Channel::setMaster(bool state) 
{
  _master = state;
  if (!_master && poll()) {
    setState(false);
  }
}

bool Channel::isMasterSet()
{
  return _master;
}

bool Channel::isOverridden()
{
  return _overridden;
}

void Channel::setRunOnce(bool state) 
{
  // TODO: Implement
  if (_master) {
    _runOnce = state;
  }
}

void Channel::override(bool state)
{
  if (_master) {
    _overridden = state;
    setState(state);
  }
}

void Channel::update()
{
  if (_master) {
    if (!_overridden) {
      bool requiredState = false;
      for (auto& interval: _intervals) {
        requiredState |= interval.activeNow();
      }

      if (_state != requiredState) {
        setState(requiredState);
      }
    }    
  }
}

}
}