#include "Client.hpp"

#include "Server.hpp"

Client::Client(const std::shared_ptr<Network::ABasicSocket>& sock, Server& serv)
  : SocketClientHelper(sock), _server(serv)
{
}

void Client::onRead([[gnu::unused]] size_t read)
{
  Network::Buffer buff;
  _readBuff.readBuffer(buff, _readBuff.getLeftRead());
  _writeBuff.writeBuffer(buff);
}

void Client::onWrite([[gnu::unused]] size_t write)
{
}
