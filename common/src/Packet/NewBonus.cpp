#include "Packet/NewBonus.hpp"

template<>
struct enum_traits<BonusTypes>
{
  static const BonusTypes enumerators[];
};

const BonusTypes enum_traits<BonusTypes>::enumerators[] = {
  BonusTypes::SPEED,
  BonusTypes::LIFE
};

namespace Packet {
NewBonus::NewBonus()
  : APacket(APacket::PacketType::NEWBONUS)
{

}

NewBonus::NewBonus(size_t entityId, uint16_t xpos, BonusTypes bt)
  : APacket(APacket::PacketType::NEWBONUS), _entityId(entityId), _xPos(xpos), _bt(bt)
{

}

std::string NewBonus::to_bytesNoHeader() const
{
  std::string ret = "";
  fill_bytes(ret, static_cast<uint16_t>(_entityId));
  fill_bytes(ret, static_cast<uint16_t>(_xPos));
  fill_bytes(ret, static_cast<uint16_t>(_bt));
  return ret;
}

std::size_t NewBonus::from_bytes(const std::string &bytes)
{
  std::size_t pos = 0;
  uint16_t		tmp;

  get_bytes(bytes, pos, _entityId);
  get_bytes(bytes, pos, _xPos);
  get_bytes(bytes, pos, tmp);
  try {
      _bt = checkEnum<decltype(_bt)>(tmp);
    }
  catch (std::runtime_error& e)
    {}
  return pos;
}
};
