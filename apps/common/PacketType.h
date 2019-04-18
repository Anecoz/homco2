#pragma once

#include <cstdint>

namespace homco2 {
namespace common {

// 1 byte for type (first byte in header)
enum class PacketType : std::uint8_t
{
  HelloPacket = 0,
  DataPacket = 1,
  MasterStateChangePacket = 2
};

}
}