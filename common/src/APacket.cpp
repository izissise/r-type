#include "APacket.hpp"

template<>
struct enum_traits<Packet::APacket::PacketType>
{
  static const Packet::APacket::PacketType enumerators[];
};

const Packet::APacket::PacketType enum_traits<Packet::APacket::PacketType>::enumerators[] = {
  Packet::APacket::PacketType::SHORTRESPONSE,
  Packet::APacket::PacketType::HANDSHAKE,
  Packet::APacket::PacketType::GETLISTROOM,
  Packet::APacket::PacketType::CREATEROOM,
  Packet::APacket::PacketType::JOINROOM,
  Packet::APacket::PacketType::ASKLISTROOM,
  Packet::APacket::PacketType::UNKNOW
};

namespace Packet {

APacket::APacket(PacketType type)
  : _type(type)
{
}

APacket::operator std::string()
{
  return to_bytes();
}

std::string APacket::to_bytes() const
{
  std::string res;
  fill_bytes(res, fromPacketType(_type));
  return res + to_bytesNoHeader();
}

Packet::APacket::PacketType APacket::toPacketType(uint16_t p)
{
  try {
      return checkEnum<PacketType>(p);
    }
  catch (std::runtime_error& e)
    {}
  return PacketType::UNKNOW;
}

Packet::APacket::PacketType APacket::toPacketType(const std::string& buff)
{
  size_t i;
  uint16_t res;

  for (i = 0; i < sizeof(res) && i != buff.size(); ++i)
    res = ((res << 8) | buff[i]);
  if (i != sizeof(res))
    return PacketType::UNKNOW;
  return toPacketType(res);
}

std::string& operator<<(std::string& a, const APacket& p)
{
  a = p.to_bytes();
  return a;
}

APacket& operator>>(const std::string& a, APacket& p)
{
  p.from_bytes(a);
  return p;
}

std::string& operator>>(const APacket& p, std::string& a) {return (a << p);}
APacket& operator<<(APacket& p, const std::string& a) {return (a >> p);}

};
