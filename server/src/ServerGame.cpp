#include "ServerGame.hpp"

#include <iostream>

#include "ClientLobby.hpp"

ServerGame::ServerGame(const ServerRoom& gameInfo)
  : _runGame(true),
    _net(Network::NetworkFactory::createNetwork())
{
  for (auto& i : gameInfo.getPlayerList())
    {
      std::string addr = i->getConnectionAddress();
      std::cout << "New game on: ";
      std::shared_ptr<Network::AListenSocket> udpListener(Network::NetworkFactory::createListenSocket(addr, "", Network::ASocket::SockType::UDP, true));
      std::cout << udpListener->getListeningIpAddr() << ":" << udpListener->getListeningPort() << " ";
      udpListener->setNewConnectionCallback(std::bind(&ServerGame::joinGame, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
      _net->registerListener(udpListener);
      std::cout << std::endl;
    }
}

void ServerGame::run()
{
  while (_runGame)
    {
      _net->poll();
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void ServerGame::joinGame(const std::weak_ptr<Network::AListenSocket>& that,
                          const std::shared_ptr<Network::Identity>& id, const Network::Buffer& data)
{
  auto listener = that.lock();
  std::shared_ptr<ClientGame> cg(new ClientGame(id, that));
  cg->readData(data);

  _clients.push_back(cg);
  std::cout << "New gameClient: " << id->ip << ":" << id->port << std::endl;
}
