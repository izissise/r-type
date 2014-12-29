#include "ServerGame.hpp"

#include <iostream>
#include <tuple>
#include <sstream>

#include "ClientLobby.hpp"
#include "Packet/GetListPlayer.hpp"
#include "Packet/StartGame.hpp"
#include "Packet/ShortResponse.hpp"
#include "Packet/NewBonus.hpp"
#include "Packet/NewMonster.hpp"

std::chrono::duration<double> ServerGame::_timeBeforeStart(5);

ServerGame::ServerGame(const ServerRoom& gameInfo, const std::string& port,
                       const DynamicLibrary::DLManager<ILibMonster>& dynlibMonsters)
  : _monsterRessouces(dynlibMonsters), _runGame(true), _started(false),
    _net(Network::NetworkFactory::createNetwork()), _entityId(100)
{
  _listeningPort = port;
  std::cout << "New game on: ";
  for (auto& i : gameInfo.getPlayerList())
    {
      std::string addr = i->getConnectionAddress();
      try {
          std::shared_ptr<Network::AListenSocket> udpListener(Network::NetworkFactory::createListenSocket(addr,
              (_listeningPort == "") ? "" : _listeningPort, Network::ASocket::SockType::UDP, true));
          if (_listeningPort == "")
            _listeningPort = [&udpListener]() {std::stringstream ss; ss << udpListener->getListeningPort(); return ss.str();}();
          std::cout << udpListener->getListeningIpAddr() << ":" << udpListener->getListeningPort() << " ";
          udpListener->setNewConnectionCallback(std::bind(&ServerGame::joinGame, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
          udpListener->setRecvFromSize(4096);
          _net->registerListener(udpListener);
          _udpListener.push_back(udpListener);
        }
      catch (Network::Error& e)
        {}
    }
  std::cout << std::endl;
}

void ServerGame::run()
{
  unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine random(seed);
  size_t op = 0;
  std::chrono::time_point<std::chrono::system_clock> start, end;
  start = std::chrono::system_clock::now();
  while (_runGame && op < 9547)
    {
      _net->poll();
      std::this_thread::sleep_for(std::chrono::milliseconds(16));
      if (!_started)
        {
          end = std::chrono::system_clock::now();
          std::chrono::duration<double> elapsedSeconds = end - start;
          if (elapsedSeconds > _timeBeforeStart)
            {
              broadcastPacket(Packet::StartGame(0, 0));
              _started = true;
            }
        }
      if ((random() % 50) == 0 && _started)
        {
          std::shared_ptr<AEntity> newEnt;
          if (random() % 10 == 0)
            {
              Bonus* b(new Bonus(_entityId, random() % 100, BonusTypes::LIFE));
              newEnt = std::shared_ptr<AEntity>(b);
              broadcastPacket(Packet::NewBonus(b->getNetId(), b->getXPos(), BonusTypes::LIFE));
            }
          else
            {
              Monster* b(new Monster(_entityId, random() % 100, createMonsterNumberX(random())));
              newEnt = std::shared_ptr<AEntity>(b);
              broadcastPacket(Packet::NewMonster(b->getNetId(), b->getXPos(), b->getName(), b->getLife()));
            }
          newEntity(newEnt);
        }
      op++;
    }
  end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsedSeconds = end - start;
  std::cout << "Game finished, time: " << elapsedSeconds.count() << "s." << std::endl;
}

void ServerGame::joinGame(const std::weak_ptr<Network::AListenSocket>& that,
                          const std::shared_ptr<Network::Identity>& id, const Network::Buffer& data)
{
  auto listener = that.lock();
  auto cg = std::make_shared<ClientGame>(id, that, *this);
  if (!_started)
    {
      cg->readData(data);

      _clients.push_back(cg);
      std::cout << "New gameClient: " << id->ip << ":" << id->port << std::endl;
      std::vector<Packet::PlayerClient> tmpList;
      _net->registerIdentity(id);
      for (auto& i : _clients)
        tmpList.push_back({i->getLogin(), static_cast<uint16_t>(i->getId())});
      broadcastPacket(Packet::GetListPlayer(tmpList));
    }
  else
    cg->sendPacket(Packet::ShortResponse(0));
}

void ServerGame::broadcastPacket(const Packet::APacket& pack) const
{
  for (auto& i : _clients)
    {
      i->sendPacket(pack);
    }
}

void ServerGame::broadcastPacketToOther(const Packet::APacket& pack, const std::shared_ptr<ClientGame>& sender) const
{
  for (auto& i : _clients)
    {
      if (i != sender)
        i->sendPacket(pack);
    }
}

std::unique_ptr<ILibMonster> ServerGame::createMonsterNumberX(size_t x) const
{
  auto monsters = _monsterRessouces.getLoadedModulesNames();
  return _monsterRessouces.createModule(monsters.at(x % monsters.size()));
}

void ServerGame::newEntity(const std::shared_ptr<AEntity>& ent)
{
  _entityId += 1;
  _entitiesList.push_back(ent);
}

