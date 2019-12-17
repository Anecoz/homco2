#pragma once

#include "../common/ChannelId.h"
#include "../common/ChannelState.h"
#include "../common/WeekdayInterval.h"

#include <vector>

namespace homco2 {
namespace server {

class Gpio;

class Channel
{
public:  
  Channel(common::ChannelId channelId, Gpio* gpio);
  ~Channel();

  Channel() = delete;

  void setMaster(bool state);
  void setRunOnce(bool state);
  void override(bool state);

  void update();

  bool poll();

  bool isMasterSet();
  bool isOverridden();

  void setIntervals(std::vector<common::WeekdayInterval>&& intervals);
  std::vector<common::WeekdayInterval> intervals();

  bool dirty();
  void clean();
  common::ChannelState state();

  common::ChannelId id() {return _channelId;}

private:
  // Turn us on or off.
  void setState(bool state);

  common::ChannelId _channelId;
  Gpio* _gpio;

  // Master switch, i.e. enabled at all or not.
  bool _master;

  // If we are overriden or not.
  bool _overridden;

  // Channel state, on or off.
  bool _state;

  // Is channel in Run Once mode, i.e. on until the next off is scheduled.
  bool _runOnce;

  bool _dirty;
  std::vector<common::WeekdayInterval> _intervals;
};

}
}