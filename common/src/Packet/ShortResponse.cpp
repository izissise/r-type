#include "Packet/ShortResponse.hpp"

namespace Packet {
  ShortResponse::ShortResponse()
  : APacket(PacketType::SHORTRESPONSE), _response(0)
  {

  }

  ShortResponse::ShortResponse(uint8_t response)
  : APacket(PacketType::SHORTRESPONSE), _response(response)
  {

  }

  std::string ShortResponse::to_bytesNoHeader() const
  {
    std::string ret = "";
    fill_bytes(ret, _response);
    return ret;
  }

  size_t ShortResponse::from_bytes(const std::string &bytes)
  {
    size_t pos = 0;
    get_bytes(bytes, pos, _response);
    return pos;
  }

  uint8_t ShortResponse::getResponse() const
  {
    return _response;
  }

};