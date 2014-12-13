#include "ServerRoom.hpp"

#include <algorithm>

ServerRoom::ServerRoom(const std::string& name, size_t id, size_t nbPMax)
  : _id(id), _name(name), _nbPMax(nbPMax)
{
}


void ServerRoom::addPlayer(const std::shared_ptr<Client>& p)
{
  _clients.push_back(p);
}

void ServerRoom::removePlayer(const std::shared_ptr<Client>& p)
{
  _clients.erase(std::remove_if(_clients.begin(), _clients.end(),
  [&p](std::shared_ptr<Client>& cl) -> bool {
    return (cl == p);
  }), _clients.end());
}
