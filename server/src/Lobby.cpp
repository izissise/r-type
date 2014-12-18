#include "Lobby.hpp"

Lobby::Lobby()
  : _roomId(0)
{

}

size_t Lobby::newRoom(const t_room& r)
{
  _rooms.insert(std::pair<size_t, ServerRoom>(_roomId, ServerRoom(r.name, _roomId, r.playerMax)));
  _roomId++;
  return _roomId - 1;
}

std::vector<t_room> Lobby::roomLists() const
{
  std::vector<t_room> roomList;

  for (auto it = _rooms.begin(), end = _rooms.end(); it != end; ++it)
    {
      t_room ro;

      ro.id = (it->second).getId();
      ro.name = (it->second).getName();
      ro.nbPlayer = (it->second).getNbPlayers();
      ro.playerMax = (it->second).getMaxNbPlayer();
      roomList.push_back(ro);
    }
  return roomList;
}

bool Lobby::joinRoom(const std::shared_ptr<Client>& cli, size_t roomId)
{
  try {
      return (_rooms.at(roomId)).addPlayer(cli);
    }
  catch (std::exception& e)
    {
      return false;
    }
}

void Lobby::leaveRoom(const std::shared_ptr<Client>& cli, size_t roomId)
{
  try {
      ServerRoom& tmp = (_rooms.at(roomId));
      tmp.removePlayer(cli);
      if (tmp.getNbPlayers() == 0)
        {
          auto it = _rooms.begin();
          if ((it = std::find_if(it, _rooms.end(),
          [roomId](const std::pair<size_t, ServerRoom>& p) {return (roomId == p.first);})) != _rooms.end())
          {
            _rooms.erase(it);
          }
        }
    }
  catch (std::exception& e)
    {}
}
