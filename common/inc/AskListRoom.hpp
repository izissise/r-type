#ifndef _ASKLISTROOM_HPP_
# define _ASKLISTROOM_HPP_

# include "APacket.hpp"

namespace Packet {
  class AskListRoom: public APacket
  {
  public:
    AskListRoom();
    ~AskListRoom();

    std::string to_bytesNoHeader() const override;
    size_t from_bytes(const std::string &bytes) override;
  private:

  };
};

#endif