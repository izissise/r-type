#include "Packet/NewMonster.hpp"

namespace Packet {
  NewMonster::NewMonster()
  : APacket(PacketType::NEWMONSTER)
  {

  }

  NewMonster::NewMonster(size_t id, const std::string& name, int life)
  : APacket(PacketType::NEWMONSTER), _name(name), _life(life), _Id(id)
  {

  }

  std::string NewMonster::to_bytesNoHeader() const
  {
    std::string ret = "";
    fill_bytes(ret, static_cast<uint16_t>(_name.length()));
    ret += _name;
    fill_bytes(ret, _life);
    fill_bytes(ret, _Id);
    return (ret);
  }

  size_t NewMonster::from_bytes(const std::string &bytes)
  {
    size_t pos = 0;
    uint16_t    loginLength = 0;

    get_bytes(bytes, pos, loginLength);
    for (; pos < bytes.length() && _name.size() < loginLength; ++pos)
      _name += bytes[pos];
    if (_name.length() != loginLength)
      throw (APacket::PackerParsingError("Parse Failed: the name size is wrong"));
	get_bytes(bytes, pos, _life);
	get_bytes(bytes, pos, _Id);
    return pos;
  }

};
