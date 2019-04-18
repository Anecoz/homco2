#pragma once

#include "ChannelId.h"

namespace homco2 {
namespace server {

class Gpio
{
public:
  Gpio();
  ~Gpio() = default;

  bool poll(ChannelId channelid) const;
  void set(ChannelId, bool state) const;
};

}
}