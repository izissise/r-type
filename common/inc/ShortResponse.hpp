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

    std::string to_bytesNoHeader() const override;
    size_t from_bytes(const std::string &bytes) override;

  private:
    uint8_t   _response;
  };
};

#endif