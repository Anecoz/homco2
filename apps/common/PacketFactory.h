#pragma once

#include "IPacket.h"
#include "PacketHeader.h"

#include <memory>

namespace homco2 {
namespace common {

class PacketFactory
{
public:
  static std::unique_ptr<IPacket> createFromRawData(const PacketHeader& header, std::unique_ptr<std::uint8_t[]> rawData, std::size_t rawSize);

  PacketFactory() = delete;
  ~PacketFactory() = delete;
};

}
}