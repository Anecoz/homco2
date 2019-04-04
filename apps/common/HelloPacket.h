#pragma once

#include "IPacket.h"

#include <string>

namespace homco2 {
namespace common {

class HelloPacket : public IPacket
{
public:
  HelloPacket(const std::string& message);
  HelloPacket(const char* rawData, std::size_t size);
  ~HelloPacket();

  //virtual std::unique_ptr<IPacket> constructFromRawData(const char* data, std::size_t size) override;
  virtual const char* getRawData() override;
  virtual std::size_t getDataSize() override;
  virtual PacketHeader createHeader() override;

  std::string message() { return _message; }

private:
  std::string _message;
};

}
}