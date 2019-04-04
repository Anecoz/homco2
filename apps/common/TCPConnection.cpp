#include "TCPConnection.h"

#include "HelloPacket.h"
#include "DataPacket.h"
#include "PacketHeader.h"
#include "PacketFactory.h"

#include <iostream>

namespace homco2 {
namespace common {

void TCPConnection::start()
{
  readAsync();
}
  
void TCPConnection::writeAsync(std::shared_ptr<IPacket> packet)
{
  auto header = packet->createHeader();
  auto rawHeader = header.getRawData();
  auto packetSize = header.getSizeOfPacket();
  auto headerSize = header.headerByteSize();

  char* buffer = new char[packetSize + headerSize];
  memcpy(buffer, rawHeader.get(), headerSize);
  memcpy(buffer + headerSize, packet->getRawData(), packetSize);

  asio::async_write(
    _socket,
    asio::buffer(buffer, headerSize + packetSize),
    std::bind(&TCPConnection::handleWrite, this, buffer, headerSize + packetSize, std::placeholders::_1, std::placeholders::_2));
}

void TCPConnection::handleWrite(const char* rawData, std::size_t rawDataSize, const asio::error_code& error, std::size_t bytesTransferred)
{
  if (error) {
    std::cerr << "A connection got an error while writing a packet: " << error.message() << std::endl;
  }
  std::cout << "A connection wrote " << std::to_string(bytesTransferred) << " bytes on socket." << std::endl;
  delete[] rawData;
}

void TCPConnection::readAsync()
{
  asio::async_read(
    _socket,
    _readBuf.prepare(PacketHeader::headerByteSize()),
    std::bind(&TCPConnection::handleReadHeader, this, std::placeholders::_1));
}

void TCPConnection::handleReadHeader(const asio::error_code& error)
{
  if (error == asio::error::eof) {
    // ok, try again
    readAsync();
    return;
  }
  else if (error == asio::error::connection_reset) {
    std::cout << "A connection was reset. Not re-reading." << std::endl;
    return;
    // TODO: Flag our connection with dead so it can be removed by server?
  }

  if (!error) {
    auto headerSize = PacketHeader::headerByteSize();

    _readBuf.commit(headerSize);
    std::istream headerStream(&_readBuf);
    char* buffer = new char [headerSize];
    headerStream.read(buffer, headerSize);
    PacketHeader packetHeader(buffer);
    delete[] buffer;

    auto packetSize = packetHeader.getSizeOfPacket();
    asio::error_code packetError;
    asio::read(
      _socket,
      _readBuf.prepare(packetSize),
      packetError);
    
    if (packetError) {
      std::cerr << "A connection received error while reading content of packet: " << packetError.message() << std::endl;
    }

    _readBuf.commit(packetSize);

    std::istream packetStream(&_readBuf);
    char* packetBuffer = new char [packetSize];
    packetStream.read(packetBuffer, packetSize);
    
    // TODO: Create packet from raw data!
    std::unique_ptr<std::uint8_t[]> rawData = std::make_unique<std::uint8_t[]>(packetSize);
    memcpy(rawData.get(), packetBuffer, packetSize);
    auto packet = PacketFactory::createFromRawData(packetHeader, std::move(rawData), packetSize);

    if (auto helloPacket = dynamic_cast<HelloPacket*>(packet.get())) {
      std::cout << "Received a hello packet, message is: " << helloPacket->message() << std::endl;
    }
    else if (auto dataPacket = dynamic_cast<DataPacket*>(packet.get())) {
      std::cout << "Received a data packet, data is: " << std::to_string(dataPacket->data()) << std::endl;
    }

    delete[] packetBuffer;
  }
  else {
    std::cerr << "A connection received error while reading header of packet: " << error.message() << std::endl;
  }
  readAsync();
}

}
}