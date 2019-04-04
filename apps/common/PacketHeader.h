#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>

namespace homco2 {
namespace common {

// 1 byte for type (first byte in header)
enum class PacketType : std::uint8_t
{
  HelloPacket = 0,
  DataPacket = 1
};

class PacketHeader
{
public:
  static std::size_t headerByteSize() { return sizeof(PacketType) + 1; }

  PacketHeader() = delete;

  PacketHeader(const char* rawData)
  {
    // First byte is the PacketType, second byte is size of rest of packet
    _type = static_cast<PacketType>(rawData[0]);
    _packetSize = rawData[1];
  }

  PacketHeader(PacketType type, std::uint8_t packetSize) 
    : _type(type)
    , _packetSize(packetSize)
  {}

  std::unique_ptr<std::uint8_t[]> getRawData()
  { 
    auto ptr = std::make_unique<std::uint8_t[]>(headerByteSize());
    ptr[0] = static_cast<char>(_type);
    ptr[1] = _packetSize;
    return ptr;
  }

  std::uint8_t getSizeOfPacket() { return _packetSize; }
  PacketType getTypeOfPacket() const { return _type; }

private:
  PacketType _type;
  std::uint8_t _packetSize;
};

}
}