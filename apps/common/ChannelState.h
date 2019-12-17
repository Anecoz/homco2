#pragma once

#include "ChannelId.h"

namespace homco2 {
namespace common {

struct ChannelState
{
  ChannelId _id;
  bool _state;
  bool _master;
  bool _overridden;
};

}
}