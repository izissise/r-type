#include "Server.hpp"

Server::Server(const std::string& addr, const std::string& port)
  : _net(Network::NetworkFactory::createNetwork()),
    _listener(Network::NetworkFactory::createListenSocket(addr, port, Network::ASocket::SockType::UDP, true))
{
  _net->registerListener(_listener);
  std::cout << "Server listening on: " << _listener->getListeningIpAddr() << ":"
            << _listener->getListeningPort() << std::endl;
}

Server::~Server()
{

}


