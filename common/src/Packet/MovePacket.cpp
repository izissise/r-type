#include <sstream>
#include <iostream>
#include "Packet/MovePacket.hpp"

namespace Packet {
  MovePacket::MovePacket()
    : APacket(Packet::APacket::PacketType::MOVE), _playerId(0), _axis(0), _speed(0)
  {

  }

  MovePacket::MovePacket(uint16_t playerId, uint8_t axis, float speed)
    : APacket(Packet::APacket::PacketType::MOVE), _playerId(playerId), _axis(axis), _speed(speed)
  {

  }

  uint8_t MovePacket::getAxis() const
  {
    return (_axis);
  }

  float MovePacket::getSpeed() const
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
	std::string tmp;
	std::stringstream ss("");
    fill_bytes(ret, _playerId);
    fill_bytes(ret, _axis);
	ss << _speed;
	tmp = ss.str();
    fill_bytes(ret, static_cast<uint16_t>(tmp.size()));
	ret += tmp;
    return (ret);
  }

  size_t MovePacket::from_bytes(const std::string &bytes)
  {
	  std::stringstream ss("");
    std::string tmp = "";
	uint16_t size;
    std::size_t pos = 0;
    get_bytes(bytes, pos, _playerId);
    get_bytes(bytes, pos, _axis);
    get_bytes(bytes, pos, size);
	for (uint16_t i = 0; i < size && pos + i < bytes.size(); ++i)
		tmp += bytes[pos + i];
	pos += size;
	ss << tmp;
	ss >> _speed;
    return pos;
  }
}