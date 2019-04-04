#pragma once

#include "IPacket.h"

#include <asio.hpp>

#include <queue>
#include <memory>
#include <vector>

namespace homco2 { namespace common { class TCPConnection; } }

namespace homco2 {
namespace server {

// This should work like: Have a queue of packets to be sent.. once per update sent the queue out and read new incoming packets.
// Define packets that are recognized by headers, transform them into c++ objects and use them, both on server and on client.
// Abstract sending packets by accepting handling a queue of IPacket and a thread-safe way to add IPackets to this queue.

class TCPServer
{
public:
  TCPServer();
  ~TCPServer();

  void addToQueue(std::unique_ptr<common::IPacket> packet);
  void flush();
private:
  void startAccept();
  
  void handleAccept(std::shared_ptr<common::TCPConnection> newConnection, const asio::error_code& error);

  asio::io_service _ioService;
  asio::ip::tcp::acceptor _acceptor;

  std::vector<std::shared_ptr<common::TCPConnection>> _connections;
  std::queue<std::unique_ptr<common::IPacket>> _queue;
};

}
}