#ifdef TEST
// Test implementation

#include "Gpio.h"

#include <iostream>
#include <string>

bool g_channels[8];

namespace homco2 {
namespace server {

Gpio::Gpio()
{
  for (unsigned i = 0; i < 8; ++i) {
    g_channels[i] = false;
  }
  std::cout << "Gpio initialized" << std::endl;
}

bool Gpio::poll(common::ChannelId channelId) const
{
  std::cout << "Gpio was asked for state of channel " << std::to_string(channelId) << ", it is " << std::to_string(g_channels[channelId]) << std::endl;
  return g_channels[channelId];
}

void Gpio::set(common::ChannelId channelId, bool state) const
{
  std::cout << "Gpio is setting channelId " << std::to_string(channelId) << " to " << std::to_string(state) << std::endl;
  g_channels[channelId] = state;
}

}
}

#else
// Actual target hw implementation.
#endif

