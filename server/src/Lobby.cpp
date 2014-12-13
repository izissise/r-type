#include "Lobby.hpp"

#include <iostream>

Lobby::Lobby()
  : _roomId(0)
{

}

void Lobby::newRoom(const t_room& r)
{
  _rooms.insert(std::pair<size_t, ServerRoom>(_roomId, ServerRoom(r.name, _roomId, r.playerMax)));
  std::cout << "New room: " << r.name << std::endl;
  _roomId++;
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
