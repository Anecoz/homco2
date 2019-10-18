#include "Channel.h"
#include "Gpio.h"

namespace homco2 {
namespace server {

Channel::Channel(ChannelId channelId, Gpio* gpio)
  : _channelId(channelId)
  , _gpio(gpio)
  , _master(false)
  , _state(false)
  //, _runOnce(false)  
{
  _state = _gpio->poll(_channelId);
}

Channel::~Channel()
{}

void Channel::setState(bool state)
{
  _gpio->set(_channelId, state);
  _state = state;
}

void Channel::setMaster(bool state) 
{
  _master = state;
  // TODO: THIS IS WRONG AND WON'T WORK. THINK IT THROUGH.
  setState(_master);
}

void Channel::update()
{
  // Check the time and figure out if we need to turn ourselves on or off.
  auto now = common::Timestamp::now();

  bool shouldBeActivated = false;
  for (const auto& interval: _activatedIntervals) {
    if (interval.intersects(now)) {
      shouldBeActivated = true;
    }
  }

  if (shouldBeActivated && !_state) {
    // We are off but should be on
    setState(true);
  }
  else if (!shouldBeActivated && _state) {
    // We are on but should now be off
    setState(false);
  }
}

}
}