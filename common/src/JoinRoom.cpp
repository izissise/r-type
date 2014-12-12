#include "JoinRoom.hpp"

namespace Packet {
  JoinRoom::JoinRoom(uint16_t idRoom)
  : APacket(PacketType::JOINROOM), _idRoom(idRoom)
  {
    
  }
  
  const std::string JoinRoom::to_bytes_body() const
  {
    std::string ret = "";
    fill_bytes(ret, _idRoom);
    return ret;
  }
  
  void JoinRoom::from_bytes_body(const std::string &bytes)
  {
    std::size_t pos = 0;
    get_bytes(bytes, pos, _idRoom);
  }
  
  uint16_t JoinRoom::getRoomId() const
  {
    return _idRoom;
  }

}