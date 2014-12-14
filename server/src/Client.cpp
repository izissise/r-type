#include "Client.hpp"

#include "Server.hpp"

Client::Client(const std::shared_ptr<Network::ABasicSocket>& sock, Server& serv)
  : SocketClientHelper(sock), _server(serv)
{
}

void Client::onRead(size_t)
{
  Network::Buffer buff;
  _readBuff.readBuffer(buff, _readBuff.getLeftRead());
  _writeBuff.writeBuffer(buff);
}

void Client::onWrite(size_t)
{
}
