#include "ServerRoom.hpp"

#include <algorithm>

ServerRoom::ServerRoom(const std::string& name, size_t id, size_t nbPMax)
  : _id(id), _name(name), _nbPMax(nbPMax)
{
}


bool ServerRoom::addPlayer(const std::shared_ptr<Client>& p)
{
  if (getNbPlayers() < _nbPMax)
    {
      _clients.push_back(p);
      return true;
    }
  return false;
}

void ServerRoom::removePlayer(const std::shared_ptr<Client>& p)
{
  _clients.erase(std::remove_if(_clients.begin(), _clients.end(),
  [&p, this](std::shared_ptr<Client>& cl) -> bool {
    return (cl == p);
  }), _clients.end());
}

void ServerRoom::broadcastAPacket(const Packet::APacket& pack)
{
  for (auto& i : _clients)
    {
      i->sendPacket(pack);
    }
}
