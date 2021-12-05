#pragma once

#include "ChannelId.h"
#include "WeekdayInterval.h"

#include <sstream>
#include <string>

#include <cpprest/json.h>

namespace homco2 {
namespace common {

struct ChannelState
{
  ChannelState()
    : _id(100000)
    , _state(false)
    , _master(false)
    , _overridden(false)
  {}

  ChannelId _id;
  bool _state;
  bool _master;
  bool _overridden;

  std::vector<WeekdayInterval> _intervals;

  std::string debugStr() const
  {
    std::stringstream ss;
    ss << "  ID: " << std::to_string(_id) << std::endl <<
    "  STATE: " << std::to_string(_state) << std::endl << 
    "  MASTER: " << std::to_string(_master) << std::endl <<
    "  OVERRIDEN: " << std::to_string(_overridden) << std::endl;

    return ss.str();
  }
};

ChannelState channelStateFromJson(web::json::value json);
web::json::value channelStateToJson(const ChannelState& channelState);

}
}