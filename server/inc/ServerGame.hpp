#ifndef SERVERGAME_H
#define SERVERGAME_H

#include <vector>
#include <chrono>
#include <thread>
#include <memory>

#include "ClientGame.hpp"
#include "ServerRoom.hpp"
#include "NetworkFactory.hpp"

class ServerGame
{
public:
  ServerGame(const ServerRoom& gameInfo);
  virtual ~ServerGame() = default;

  void run();

  const std::shared_ptr<Network::AListenSocket>& getNetworkInfo() const {return _udpListener;};

protected:
  void joinGame(const std::weak_ptr<Network::AListenSocket>& that,
                const std::shared_ptr<Network::Identity>& id, const Network::Buffer& data);

protected:
  bool									   _runGame;
  std::unique_ptr<Network::ANetwork>       _net;
  std::shared_ptr<Network::AListenSocket>  _udpListener;
  std::vector<std::shared_ptr<ClientGame>> _clients;
};

#endif // SERVERGAME_H
