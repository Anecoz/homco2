#pragma once

#include "IPacket.h"

#include <string>

namespace homco2 {
namespace common {

class DataPacket : public IPacket
{
public:
  DataPacket(int data);
  DataPacket(const char* rawData, std::size_t size);
  ~DataPacket();

  //virtual std::unique_ptr<IPacket> constructFromRawData(const char* data, std::size_t size) override;
  virtual const char* getRawData() override;
  virtual std::size_t getDataSize() override;
  virtual PacketHeader createHeader() override;

  int data() { return _data; }

private:
  int _data;
};

}
}