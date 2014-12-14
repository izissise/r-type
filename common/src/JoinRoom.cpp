#include "JoinRoom.hpp"

namespace Packet {
  uint16_t JoinRoom::headerNumber = static_cast<uint16_t>(APacket::PacketType::JOINROOM);

  JoinRoom::JoinRoom()
  : APacket(PacketType::JOINROOM), _idRoom(0)
  {

  }

  JoinRoom::JoinRoom(uint16_t idRoom)
  : APacket(PacketType::JOINROOM), _idRoom(idRoom)
  {

  }

  std::string JoinRoom::to_bytes_body() const
  {
    std::string ret = "";
    fill_bytes(ret, _idRoom);
    return ret;
  }

  std::size_t JoinRoom::from_bytes_body(const std::string &bytes)
  {
    std::size_t pos = _begin;
    get_bytes(bytes, pos, _idRoom);
    return (pos - _begin);
  }

  uint16_t JoinRoom::getRoomId() const
  {
    return _idRoom;
  }

};
