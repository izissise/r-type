#include "ServerRoom.hpp"

#include <algorithm>

ServerRoom::ServerRoom(const std::string& name, size_t id, size_t nbPMax)
  : _id(id), _name(name), _nbPMax(nbPMax), _nbPlayer(0)
{
}


bool ServerRoom::addPlayer(const std::shared_ptr<Client>& p)
{
  if (_nbPlayer < _nbPMax)
    {
      _clients.push_back(p);
      _nbPlayer += 1;
      return true;
    }
  return false;
}

void ServerRoom::removePlayer(const std::shared_ptr<Client>& p)
{
  _clients.erase(std::remove_if(_clients.begin(), _clients.end(),
  [&p, this](std::shared_ptr<Client>& cl) -> bool {
    bool isThis = (cl == p);
    if (isThis)
      _nbPlayer -= 1;
    return isThis;
  }), _clients.end());
}
