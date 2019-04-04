#include "NetworkManager.h"

#include "HelloPacket.h"
#include "DataPacket.h"

#include <iostream>

namespace homco2 {
namespace network {

NetworkManager::NetworkManager()
  : _ioService()
  , _resolver(_ioService)
  , _connection(common::TCPConnection::create(_ioService))
{
  try {
    std::cout << "Attempt to connect to server... ";

    asio::ip::tcp::resolver::query query("localhost", "49153");
    asio::ip::tcp::resolver::iterator endpointIterator = _resolver.resolve(query);

    asio::connect(_connection->socket(), endpointIterator);
    std::cout << "Connected to server!" << std::endl;

    {
      std::cout << "Attempt to send a HelloPacket..." << std::endl;
      auto packet = std::make_shared<common::HelloPacket>("hello");
      auto header = packet->createHeader();
      std::cout << "Packet now going to write, header size is: " << std::to_string(header.headerByteSize()) << ", packet size is: " << std::to_string(header.getSizeOfPacket()) << std::endl;
      _connection->writeAsync(packet);
    }
    
    {
      std::cout << "Attempt to send another HelloPacket with message 'OMEGALUL'" << std::endl;
      auto packet = std::make_shared<common::HelloPacket>("OMEGALUL");
      auto header = packet->createHeader();
      std::cout << "Packet now going to write, header size is: " << std::to_string(header.headerByteSize()) << ", packet size is: " << std::to_string(header.getSizeOfPacket()) << std::endl;
      _connection->writeAsync(packet);
    }

    {
      std::cout << "Sending a data packet instead, data is 42" << std::endl;
      auto packet = std::make_shared<common::DataPacket>(42);
      auto header = packet->createHeader();
      std::cout << "Packet now going to write, header size is: " << std::to_string(header.headerByteSize()) << ", packet size is: " << std::to_string(header.getSizeOfPacket()) << std::endl;
      _connection->writeAsync(packet);
    }
  }
  catch (const std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
  }
}

NetworkManager::~NetworkManager()
{}

}
}