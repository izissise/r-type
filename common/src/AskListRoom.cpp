#include "AskListRoom.hpp"

namespace Packet {

  AskListRoom::AskListRoom()
  : Packet::APacket(APacket::PacketType::ASKLISTROOM)
  {

  }

  AskListRoom::~AskListRoom()
  {

  }


  std::string AskListRoom::to_bytes() const
  {
    return ("");
  }

  size_t AskListRoom::from_bytes(const std::string &)
  {
    return (0);
  }
};
