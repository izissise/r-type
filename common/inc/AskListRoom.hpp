#ifndef _ASKLISTROOM_HPP_
# define _ASKLISTROOM_HPP_

# include "APacket.hpp"

namespace Packet {
  class AskListRoom: public APacket
  {
  private:
  	static uint16_t headerNumber;

  public:
    AskListRoom();
    ~AskListRoom();

  private:
    std::string to_bytes_body() const override;
    std::size_t from_bytes_body(const std::string &bytes) override;
    uint16_t getHeaderNumber() const override {return headerNumber;};
  };
};

#endif