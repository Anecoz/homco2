#include "DataPacket.h"

namespace homco2 {
namespace common {

DataPacket::DataPacket(int data)
  : _data(data)
{}

DataPacket::DataPacket(const char* rawData, std::size_t size)
{
  if (size == getDataSize()) {
    auto ptr = reinterpret_cast<const int*>(rawData);
    _data = *ptr;
  }
}

DataPacket::~DataPacket()
{}

PacketHeader DataPacket::createHeader()
{
  return PacketHeader(PacketType::DataPacket, static_cast<std::uint8_t>(getDataSize()));
}

const char* DataPacket::getRawData()
{
  return reinterpret_cast<const char*>(&_data);
}

std::size_t DataPacket::getDataSize()
{
  return sizeof(int);
}

}
}