#include "Packet/MovePacket.hpp"

namespace Packet {
  MovePacket::MovePacket() : APacket(Packet::APacket::PacketType::MOVE), _playerId(0), _axis(0), _speed(0)
  {
    
  }
  
  MovePacket::MovePacket(uint16_t playerId, uint8_t axis, float speed) : APacket(Packet::APacket::PacketType::MOVE), _playerId(playerId), _axis(axis), _speed(speed)
  {
    
  }
  
  uint8_t MovePacket::getAxis() const
  {
    return (_axis);
  }
  
  uint16_t MovePacket::getSpeed() const
  {
    return (_speed);
  }
  
  uint16_t MovePacket::getPlayerId() const
  {
    return (_playerId);
  }
  
  std::string MovePacket::to_bytesNoHeader() const
  {
    std::string ret = "";
    fill_bytes(ret, _playerId);
    fill_bytes(ret, _axis);
    fill_bytes(ret, static_cast<uint32_t>(_speed));
    return (ret);
  }
  
  size_t MovePacket::from_bytes(const std::string &bytes)
  {
    uint32_t tmp = 0;
    std::size_t pos = 0;
    get_bytes(bytes, pos, _playerId);
    get_bytes(bytes, pos, _axis);
    get_bytes(bytes, pos, tmp);
    _speed = static_cast<float>(tmp);
    return pos;
  }
}