#pragma once

#include "ChannelId.h"
#include "../common/Interval.h"

#include <vector>

namespace homco2 {
namespace server {

class Gpio;

class Channel
{
public:  
  Channel(ChannelId channelId, Gpio* gpio);
  ~Channel();

  Channel() = delete;

  void setMaster(bool state);

  void update();

private:
  // Turn us on or off.
  void setState(bool state);

  ChannelId _channelId;
  Gpio* _gpio;

  // Master switch that overrides any stored intervals.
  bool _master;

  // Channel state, on or off.
  bool _state;

  // Is channel in Run Once mode, i.e. on until the next off is scheduled.
  //bool _runOnce;
  
  std::vector<common::Interval> _activatedIntervals;
};

}
}