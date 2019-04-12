#pragma once

#include "IPacket.h"

#include <asio.hpp>

#include <memory>
#include <mutex>
#include <vector>
#include <thread>

namespace homco2 { namespace common { class TCPConnection; } }

namespace homco2 {
namespace server {

class TCPServer
{
public:
  TCPServer();
  ~TCPServer();

  void purgeDeadConnections();

  void stop();

  std::vector<std::unique_ptr<common::IPacket>> takePackets();
private:
  // Executed on main thread.
  void internalRun();

  // Executed on _thread.
  void startAccept();

  // Executed on _thread.
  void handleAccept(std::shared_ptr<common::TCPConnection> newConnection, const asio::error_code& error);  

  asio::io_service _ioService;
  asio::ip::tcp::acceptor _acceptor;
  bool _shutdown;

  std::thread _thread;
  std::mutex _connectionsMtx;

  std::vector<std::shared_ptr<common::TCPConnection>> _connections;
};

}
}