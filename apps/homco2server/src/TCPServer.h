#pragma once

#include "IPacket.h"

#include <asio.hpp>

//#include <queue>
#include <memory>
#include <vector>

namespace homco2 { namespace common { class TCPConnection; } }

namespace homco2 {
namespace server {

class TCPServer
{
public:
  TCPServer();
  ~TCPServer();

  //void addToQueue(std::unique_ptr<common::IPacket> packet);
  //void flush();
  std::vector<std::unique_ptr<common::IPacket>> takePackets();
private:
  void startAccept();
  
  void handleAccept(std::shared_ptr<common::TCPConnection> newConnection, const asio::error_code& error);

  asio::io_service _ioService;
  asio::ip::tcp::acceptor _acceptor;

  std::shared_ptr<common::TCPConnection> _connection;
  //std::queue<std::unique_ptr<common::IPacket>> _queue;
};

}
}