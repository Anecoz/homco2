#pragma once

#include "IPacket.h"

#include <cstdint>

namespace homco2 {
namespace common {

class MasterStateChangePacket : public IPacket
{
public:
  MasterStateChangePacket() = delete;
  MasterStateChangePacket(const char* rawData, std::size_t size);
  MasterStateChangePacket(std::uint8_t channel, bool masterState);  
  ~MasterStateChangePacket();

  virtual const char* getRawData() override;
  virtual std::size_t getDataSize() override;
  virtual PacketHeader createHeader() override;

  std::uint8_t channel() { return _channel; }
  bool masterState() { return _masterState; }

private:
  std::uint8_t _channel;
  bool _masterState;
  std::uint8_t* _rawCopy;
};

}
}