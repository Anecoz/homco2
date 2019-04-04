#pragma once

#include "IPacket.h"

#include <asio.hpp>

#include <memory>
#include <vector>

namespace homco2 {
namespace common {

class TCPConnection
{
public:
  static std::shared_ptr<TCPConnection> create(asio::io_service& ioService)
  {
    return std::shared_ptr<TCPConnection>(new TCPConnection(ioService));
  }

  asio::ip::tcp::socket& socket() {return _socket;}

  void start();
  void writeAsync(std::shared_ptr<IPacket> packet);
  std::vector<std::unique_ptr<IPacket>> takeQueue() {
    std::vector<std::unique_ptr<IPacket>> temp;
    std::swap(temp, _packetQueue);
    return std::move(temp);
  }

private:
  TCPConnection(asio::io_service& ioService)
    : _socket(ioService)
  {}

  void readAsync();
  void handleReadHeader(const asio::error_code& error);

  void handleWrite(const char* rawData, std::size_t rawDataSize, const asio::error_code& error, std::size_t bytesTransferred);

  asio::streambuf _readBuf;
  asio::ip::tcp::socket _socket;
  std::vector<std::unique_ptr<IPacket>> _packetQueue;
};

}
}