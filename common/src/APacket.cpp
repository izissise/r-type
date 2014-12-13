#include "APacket.hpp"

namespace Packet {

  APacket::APacket(PacketType type)
  : _type(type)
  {
  }

/*  std::string APacket::operator<<(const APacket&)
  {
	return to_bytes_body();
  }
*/

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
}
