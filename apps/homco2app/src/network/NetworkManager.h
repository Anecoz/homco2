#pragma once

#include "TCPConnection.h"

#include <asio.hpp>

#include <memory>

namespace homco2 {
namespace network {

class NetworkManager
{
public:
  NetworkManager();
  ~NetworkManager();

private:
  asio::io_service _ioService;
  asio::ip::tcp::resolver _resolver;

  std::shared_ptr<common::TCPConnection> _connection;
};

}
}