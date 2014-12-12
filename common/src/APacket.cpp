#include "APacket.hpp"

namespace Packet {
  
  APacket::APacket(PacketType type)
  : _type(static_cast<uint8_t>(type))
  {
  }
  
  APacket::~APacket()
  {
    
  }
  
  const std::string APacket::to_bytes() const
  {
    std::string ret;
    
    ret.push_back(_type);
    ret += to_bytes_body();
    return (ret);
  }
  
  void APacket::from_bytes(const std::string &bytes)
  {
    if (bytes.empty() && bytes[0] != _type)
      throw std::invalid_argument("Error while parsing packet");
    from_bytes_body(bytes);
  }
}
