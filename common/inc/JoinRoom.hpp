#ifndef _JOINROOM_HPP_
# define _JOINROOM_HPP_

# include "APacket.hpp"

namespace Packet {
  class JoinRoom: public APacket
  {
  public:
    JoinRoom();
    JoinRoom(uint16_t idRoom);

    uint16_t getRoomId() const;

	std::string to_bytes() const override;
    size_t from_bytes(const std::string &bytes) override;

    uint16_t  _idRoom;
  };
};

#endif