#ifndef SERVERGAME_H
#define SERVERGAME_H

#include <vector>
#include <chrono>
#include <thread>
#include <memory>

#include "AMonster.hpp"
#include "ClientGame.hpp"
#include "ServerRoom.hpp"
#include "Packet/APacket.hpp"
#include "NetworkFactory.hpp"
#include "DLManager.hpp"

class ServerGame
{
private:
  static std::chrono::duration<double> _timeBeforeStart;

public:
  ServerGame(const ServerRoom& gameInfo, const std::string& port, const DynamicLibrary::DLManager<AMonster>& dynlibMonsters);
  virtual ~ServerGame() = default;

  void run();

  void newEntity();
  std::unique_ptr<AMonster> createMonsterNumberX(size_t x) const;

  const std::string& listeningPort() const {return _listeningPort;};

  void broadcastPacket(const Packet::APacket& pack) const;
  void broadcastPacketToOther(const Packet::APacket& pack, const std::shared_ptr<ClientGame>& sender) const;
protected:
  void joinGame(const std::weak_ptr<Network::AListenSocket>& that,
                const std::shared_ptr<Network::Identity>& id, const Network::Buffer& data);

protected:
  const	DynamicLibrary::DLManager<AMonster>&     	   _monsterRessouces;
  bool									               _runGame;
  bool												   _started;
  std::unique_ptr<Network::ANetwork>       			   _net;
  std::vector<std::shared_ptr<Network::AListenSocket>> _udpListener;
  std::vector<std::shared_ptr<ClientGame>>             _clients;
  std::string 										   _listeningPort;
  size_t											   _entityId;
};

#endif // SERVERGAME_H
