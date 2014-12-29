#include "Packet/EntityDie.hpp"

namespace Packet {
  EntityDie::EntityDie()
  : APacket(APacket::PacketType::ENTITYDIE)
  {

  }

  EntityDie::EntityDie(size_t entityId)
  : APacket(APacket::PacketType::ENTITYDIE), _entityId(entityId)
  {

  }

  std::string EntityDie::to_bytesNoHeader() const
  {
    std::string ret = "";
    fill_bytes(ret, static_cast<uint16_t>(_entityId));
    return ret;
  }

  std::size_t EntityDie::from_bytes(const std::string &bytes)
  {
    std::size_t pos = 0;

    get_bytes(bytes, pos, _entityId);
    return pos;
  }
};