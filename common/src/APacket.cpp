#include "APacket.hpp"

namespace Packet {

  APacket::APacket(PacketType type)
  : _type(type)
  {
  }

  APacket::APacket(const std::string& bytes)
  {

  };

  APacket::operator std::string()
  {
    return to_bytes();
  }

  std::string APacket::to_bytes() const
  {
    std::string ret;
    uint8_t		type;

    type = getHeaderNumber();
    fill_bytes(ret, type);
    ret += to_bytes_body();
    return (ret);
  }

  void APacket::from_bytes(const std::string &bytes)
  {
  	uint8_t		type;
  	uint8_t		rtype;
  	size_t	    pos = 0;

  	type = getHeaderNumber();
	get_bytes(bytes, pos, rtype);
    if (bytes.empty() && (type != rtype))
      throw std::invalid_argument("Error while parsing packet");
    from_bytes_body(bytes);
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

}
