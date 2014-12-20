#include "ServerRoom.hpp"

#include <algorithm>

#include "Server.hpp"
#include "GetListPlayer.hpp"

ServerRoom::ServerRoom(const std::string& name, size_t id, size_t nbPMax)
  : _id(id), _name(name), _nbPMax(nbPMax)
{
}


bool ServerRoom::addPlayer(const std::shared_ptr<ClientLobby>& p)
{
  if (getNbPlayers() < _nbPMax)
    {
      _clients.push_back(p);
      return true;
    }
  return false;
}

void ServerRoom::removePlayer(const std::shared_ptr<ClientLobby>& p)
{
  _clients.erase(std::remove_if(_clients.begin(), _clients.end(),
  [&p, this](std::shared_ptr<ClientLobby>& cl) -> bool {
    return (cl == p);
  }), _clients.end());
  sendPlayerList();
}

void ServerRoom::broadcastAPacket(const Packet::APacket& pack) const
{
  for (auto& i : _clients)
    {
      i->sendPacket(pack);
    }
}

void ServerRoom::sendPlayerList() const
{
  broadcastAPacket(Packet::GetListPlayer([this]() {
    std::vector<Packet::PlayerClient> tmp;
    for (auto& i : _clients)
      {
        tmp.push_back(i->getLogin());
      }
    return tmp;
  }()));
}

void ServerRoom::tryLaunchGame(Server& server) const
{
  bool ok = true;
  for (auto& i : _clients)
    {
      if (!i->isReadyForGame())
        ok = false;
    }
  if (ok)
    {
      size_t id = 0;
      uint16_t port = server.createNewGame(*this);
      for (auto& i : _clients)
        {
          i->startGame(port, id);
          id++;
        }
    }
}
