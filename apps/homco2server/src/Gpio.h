#pragma once

#include "../common/ChannelId.h"

namespace homco2 {
namespace server {

class Gpio
{
public:
  Gpio();
  ~Gpio() = default;

  bool poll(common::ChannelId channelid) const;
  void set(common::ChannelId, bool state) const;
};

}
}