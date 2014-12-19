#include "ServerGame.hpp"

#include <chrono>
#include <thread>

ServerGame::ServerGame(const ServerRoom& gameInfo, const std::string& listeningAddr)
  : _runGame(true),
    _net(Network::NetworkFactory::createNetwork()),
    _udpListener(Network::NetworkFactory::createListenSocket(listeningAddr, "", Network::ASocket::SockType::UDP, true))
{
  //_udpListener->setNewConnectionCallback();
  _net->registerListener(_udpListener);
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

}
