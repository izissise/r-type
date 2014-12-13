#ifndef _SHORTRESPONSE_HPP_
# define _SHORTRESPONSE_HPP_

# include "APacket.hpp"

namespace Packet {
  class ShortResponse: public APacket
  {
  public:
    ShortResponse();
    ShortResponse(uint8_t response);

    uint8_t getResponse() const;
  private:
    std::string to_bytes_body() const override;
    void from_bytes_body(const std::string &bytes) override;

    uint8_t   _response;
  };
}

#endif