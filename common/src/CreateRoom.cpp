#include "CreateRoom.hpp"

namespace Packet {
  uint16_t CreateRoom::headerNumber = static_cast<uint16_t>(APacket::PacketType::CREATEROOM);

  CreateRoom::CreateRoom()
  : APacket(PacketType::CREATEROOM), _room(nullptr)
  {

  }

  CreateRoom::CreateRoom(const t_room &room)
  : APacket(PacketType::CREATEROOM), _room(new t_room(room))
  {

  }

  const std::shared_ptr<t_room>  &CreateRoom::getRoom() const
  {
    return (_room);
  }

  std::string CreateRoom::to_bytes_body() const
  {
    std::string ret = "";
    fill_bytes(ret, static_cast<uint16_t>(_room->name.length()));
    ret += _room->name;
    fill_bytes(ret, _room->playerMax);
    return (ret);
  }

  std::size_t CreateRoom::from_bytes_body(const std::string &bytes)
  {
    std::size_t pos = _begin;
    uint16_t length;

    _room.reset(new t_room());
    get_bytes(bytes, pos, length);
    for (; pos < bytes.length() && pos < length + sizeof(_type) + sizeof(length); ++pos)
      _room->name += bytes[pos];
    if (pos - sizeof(_type) - sizeof(length) != length)
      throw std::runtime_error("The size of the room's name is not right");
    get_bytes(bytes, pos, _room->playerMax);
    _room->nbPlayer = 0;
    return pos - _begin;
  }
};
