#include "Client.hpp"

#include "Server.hpp"

#include <iostream>

Client::Client(const std::shared_ptr<Network::ABasicSocket>& sock, Server& serv)
  : SocketClientHelper(sock), _server(serv)
{
}

void Client::onRead(size_t readSize)
{
  Network::Buffer buff;
  std::cout << "Read = " << readSize << std::endl;
  _readBuff.readBuffer(buff, _readBuff.getLeftRead());
  _writeBuff.writeBuffer(buff);
}

void Client::onWrite(size_t writeSize)
{
  std::cout << "Write = " << writeSize << std::endl;
}
