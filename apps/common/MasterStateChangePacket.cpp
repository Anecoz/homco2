#include "MasterStateChangePacket.h"

namespace homco2 {
namespace common {

MasterStateChangePacket::MasterStateChangePacket(const char* rawData, std::size_t size)
{
  if (size == getDataSize()) {
    auto ptr = reinterpret_cast<const std::uint8_t*>(rawData);
    _channel = ptr[0];
    _masterState = static_cast<bool>(ptr[1]);

    _rawCopy = new std::uint8_t[getDataSize()];
    std::memcpy(_rawCopy, rawData, size);
  }
}

MasterStateChangePacket::MasterStateChangePacket(std::uint8_t channel, bool masterState)
  : _channel(channel)
  , _masterState(masterState)
  , _rawCopy(nullptr)
{  
  _rawCopy = new std::uint8_t[getDataSize()];
  _rawCopy[0] = _channel;
  _rawCopy[1] = static_cast<std::uint8_t>(_masterState);
}

MasterStateChangePacket::~MasterStateChangePacket()
{
  delete _rawCopy;
}

const char* MasterStateChangePacket::getRawData()
{
  return reinterpret_cast<const char*>(_rawCopy);
}

std::size_t MasterStateChangePacket::getDataSize()
{
  // 1 byte for channel and 1 byte for state
  return sizeof(std::uint8_t) + 1;
}

PacketHeader MasterStateChangePacket::createHeader()
{
  return PacketHeader(PacketType::MasterStateChangePacket, static_cast<std::uint8_t>(getDataSize()));
}

}
}