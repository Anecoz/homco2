#pragma once

#include "ChannelId.h"
#include "WeekdayInterval.h"

#include <cpprest/json.h>

namespace homco2 {
namespace common {

struct ChannelState
{
  ChannelId _id;
  bool _state;
  bool _master;
  bool _overridden;

  std::vector<WeekdayInterval> _intervals;
};

ChannelState channelStateFromJson(web::json::value json);
web::json::value channelStateToJson(const ChannelState& channelState);

}
}