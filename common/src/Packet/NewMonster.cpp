#include "Packet/NewMonster.hpp"

namespace Packet {

NewMonster::NewMonster(const std::string& name, int power, int life)
  : APacket(PacketType::NEWMONSTER), _name(name), _power(power), _life(life)
{
}

std::string NewMonster::to_bytesNoHeader() const
{
  std::string ret("");

  fill_bytes(ret, static_cast<uint32_t>(_name.size()));
  for (auto it : _name)
    {
      ret += it;
    }
  fill_bytes(ret, _power);
  fill_bytes(ret, _life);
  return (ret);
}

size_t NewMonster::from_bytes(const std::string &bytes)
{
  size_t   pos = 0;
  uint32_t nameLength = 0;

  get_bytes(bytes, pos, nameLength);
  for (; pos < bytes.length() && _name.size() < nameLength; ++pos)
    _name += bytes[pos];
  if (_name.length() != nameLength)
    throw (APacket::PackerParsingError("Parse Failed: the name length is wrong"));
  get_bytes(bytes, pos, _power);
  get_bytes(bytes, pos, _life);
  return pos;
}

};
