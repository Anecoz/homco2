#include "Channel.h"
#include "Gpio.h"

namespace homco2 {
namespace server {

Channel::Channel(common::ChannelId channelId, Gpio* gpio)
  : _channelId(channelId)
  , _gpio(gpio)
  , _master(true)
  , _overridden(false)
  , _state(false)
  , _runOnce(false)
  , _dirty(false)
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
  if (_state != state) {
    _gpio->set(_channelId, state);
    _state = state;
    _dirty = true;
  }
}

bool Channel::poll()
{
  return _gpio->poll(_channelId);
}

void Channel::setMaster(bool state) 
{
  if (_master != state) {
    _master = state;
    if (!_master && poll()) {
      setState(false);
    }
    if (!_master && _overridden) {
      _overridden = false;
    }
    _dirty = true;
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
    _dirty = true;
  }  
}

void Channel::override(bool state)
{
  if (_master) {
    if (_overridden != state) {
      _overridden = state;
      setState(state);
      _dirty = true;
    }
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

common::ChannelState Channel::state()
{
  common::ChannelState out;
  out._id = _channelId;
  out._master = _master;
  out._overridden = _overridden;
  out._state = _state;
  return out;
}

bool Channel::dirty()
{
  return _dirty;
}

void Channel::clean()
{
  _dirty = false;
}

}
}