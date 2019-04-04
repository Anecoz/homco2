#include "HelloPacket.h"

namespace homco2 {
namespace common {

HelloPacket::HelloPacket(const std::string& message)
  : _message(message)
{}

HelloPacket::HelloPacket(const char* rawData, std::size_t size)
{
  _message.insert(0, rawData, size);
}

HelloPacket::~HelloPacket()
{}

PacketHeader HelloPacket::createHeader()
{
  return PacketHeader(PacketType::HelloPacket, static_cast<uint8_t>(_message.size()));
}

/*std::unique_ptr<IPacket> HelloPacket::constructFromRawData(const char* data, std::size_t size)
{
  _message.insert(0, data, size);
}*/

const char* HelloPacket::getRawData()
{
  return _message.data();
}

std::size_t HelloPacket::getDataSize()
{
  return _message.size();
}

}
}