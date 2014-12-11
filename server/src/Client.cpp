#include "Client.hpp"

#include <iostream>

Client::Client(const std::shared_ptr<Network::ABasicSocket>& sock)
  : ClientHelper(sock)
{
}

void Client::onReadeable()
{
  Network::Buffer buff;
  std::cout << "Begin read" << std::endl;
  _socket->read(buff, 1);

  std::cout << "Read: " << buff << std::endl;
  _writeBuff.writeBuffer(buff);
  _socket->setEventRequest(Network::ASocket::Event::RDWR);
}

void Client::onWritable()
{
  Network::Buffer buff;

  _writeBuff.readBuffer(buff, 5);
  size_t read = _socket->write(buff);
  _writeBuff.rollbackReadBuffer(5 - read);
  if (_writeBuff.getLeftRead())
    _socket->setEventRequest(Network::ASocket::Event::RDWR);
  else
    _socket->setEventRequest(Network::ASocket::Event::READ);
}
