#include "AskListRoom.hpp"

namespace Packet {
    uint16_t AskListRoom::headerNumber = static_cast<uint16_t>(APacket::PacketType::ASKLISTROOM);
  
  AskListRoom::AskListRoom()
  : Packet::APacket(APacket::PacketType::ASKLISTROOM)
  {
    
  }
  
  AskListRoom::~AskListRoom()
  {
    
  }


  std::string AskListRoom::to_bytes_body() const
  {
    return ("");
  }
  
  std::size_t AskListRoom::from_bytes_body(const std::string &)
  {
    return (0);
  }
};
