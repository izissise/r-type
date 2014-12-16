#include "JoinRoom.hpp"

namespace Packet {
  JoinRoom::JoinRoom()
  : APacket(PacketType::JOINROOM), _idRoom(0)
  {

  }

  JoinRoom::JoinRoom(uint16_t idRoom)
  : APacket(PacketType::JOINROOM), _idRoom(idRoom)
  {

  }

  std::string JoinRoom::to_bytes() const
  {
    std::string ret = "";
    fill_bytes(ret, _idRoom);
    return ret;
  }

  size_t JoinRoom::from_bytes(const std::string &bytes)
  {
    size_t pos = 0;
    get_bytes(bytes, pos, _idRoom);
    return pos;
  }

  uint16_t JoinRoom::getRoomId() const
  {
    return _idRoom;
  }

};
