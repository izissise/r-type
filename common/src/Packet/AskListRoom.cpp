#include "Packet/AskListRoom.hpp"

namespace Packet {

  AskListRoom::AskListRoom()
  : Packet::APacket(APacket::PacketType::ASKLISTROOM)
  {

  }

  std::string AskListRoom::to_bytesNoHeader() const
  {
    return ("");
  }

  size_t AskListRoom::from_bytes(const std::string &)
  {
    return (0);
  }
};
