#include "TCPServer.h"

#include "TCPConnection.h"

#include <functional>
#include <algorithm>
#include <iostream>

namespace homco2 {
namespace server {

TCPServer::TCPServer()
  : _ioService()
  , _acceptor(_ioService, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 49153))
  , _shutdown(false)
{
  internalRun();  
}

TCPServer::~TCPServer()
{
}

void TCPServer::internalRun()
{
  _thread = std::thread([this]() {
    while (!_shutdown) {
      startAccept();
      _ioService.run();
    }
  });
}

void TCPServer::stop()
{
  _shutdown = true;
  _thread.join();
}

std::vector<std::unique_ptr<common::IPacket>> TCPServer::takePackets()
{
  std::vector<std::unique_ptr<common::IPacket>> output;
  {
    // TODO: A shame to be holding on to this mutex over the entire period.
    std::lock_guard<std::mutex> lock(_connectionsMtx);
    for (auto connection : _connections) {
      auto packets = connection->takeQueue();
      // Take all packets from connection and move to the output vector.
      for (std::size_t i = 0; i < packets.size(); ++i) {
        output.emplace_back(std::move(packets[i]));
      }
    }
  }  
  return std::move(output);
}

void TCPServer::purgeDeadConnections()
{
  std::lock_guard<std::mutex> lock(_connectionsMtx);
  if (!_connections.empty()) {
    auto sizeBefore = _connections.size();
    _connections.erase(std::remove_if(_connections.begin(), _connections.end(), [](const std::shared_ptr<common::TCPConnection>& conn)
    {
      return conn->dead();
    }), _connections.end());
    auto sizeAfter = _connections.size();

    if (sizeAfter < sizeBefore) {
      std::cout << "TCPServer: purged " << std::to_string(sizeBefore - sizeAfter) << " connections (they were dead)." << std::endl;
    }
  }  
}

void TCPServer::startAccept()
{
  std::shared_ptr<common::TCPConnection> newConnection = common::TCPConnection::create(_ioService);
  _acceptor.async_accept(newConnection->socket(), std::bind(&TCPServer::handleAccept, this, newConnection, std::placeholders::_1));
}

void TCPServer::handleAccept(std::shared_ptr<common::TCPConnection> newConnection, const asio::error_code& error)
{
  if (!error) {
    // Print something to log to signal that we have a new connection.
    newConnection->updateAddress();
    std::cout << "TCPServer: received a new connection: " << newConnection->readableAddress() << std::endl;
    {
      std::lock_guard<std::mutex> lock(_connectionsMtx);
      _connections.emplace_back(newConnection);
      _connections.back()->start();
    }
    startAccept();
  }
}

}
}