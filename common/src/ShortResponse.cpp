#include "ShortResponse.hpp"

namespace Packet {
  ShortResponse::ShortResponse(uint8_t response)
  : APacket(PacketType::SHORTRESPONSE), _response(response)
  {
    
  }
  
  const std::string ShortResponse::to_bytes_body() const
  {
    std::string ret = "";
    fill_bytes(ret, _response);
    return ret;
  }
  
  void ShortResponse::from_bytes_body(const std::string &bytes)
  {
    std::size_t pos = 0;
    get_bytes(bytes, pos, _response);
  }
  
  uint8_t ShortResponse::getResponse() const
  {
    return _response;
  }

}