#pragma once

#include "PacketHeader.h"

#include <cstddef>

#include <memory>

namespace homco2 {
namespace common {

class IPacket
{
public:
  virtual ~IPacket() {}
  //virtual std::unique_ptr<IPacket> constructFromRawData(const char* data, std::size_t size) = 0;
  virtual const char* getRawData() = 0;
  virtual std::size_t getDataSize() = 0;
  virtual PacketHeader createHeader() = 0;
};

}
}