#include "Server.hpp"

Server::Server(const std::string& addr, const std::string& port)
  : _net(Network::NetworkFactory::createNetwork()),
    _lobbyListener(Network::NetworkFactory::createListenSocket(addr, port, Network::ASocket::SockType::TCP, true))
{
  _lobbyListener->setAcceptorCallback(std::bind(&Server::acceptNewClient, this, std::placeholders::_1));
  _net->registerListener(_lobbyListener);
  std::cout << "Server listening on " << _lobbyListener->getListeningIpAddr() << ":"
            << _lobbyListener->getListeningPort() << std::endl;
}

void Server::run()
{
  while (1)
    _net->poll(true);
}

void Server::acceptNewClient(const std::weak_ptr<Network::AListenSocket>& that)
{
  std::shared_ptr<Network::AListenSocket> listener = that.lock();
  std::shared_ptr<Network::ABasicSocket> nClientSock = listener->acceptClient();
  std::shared_ptr<Client> nclient(new Client(nClientSock));

  nClientSock->setReadeableCallback(std::bind(&Client::onReadeable, nclient));
  nClientSock->setWritableCallback(std::bind(&Client::onWritable, nclient));

  _clients.push_back(nclient);
  _net->registerClient(nClientSock);
  std::cout << "Hello from: " << nClientSock->getRemoteIpAddr() << ":" << nClientSock->getRemotePort() << std::endl;
}
