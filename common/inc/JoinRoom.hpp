#ifndef _JOINROOM_HPP_
# define _JOINROOM_HPP_

# include "APacket.hpp"

namespace Packet {
  class JoinRoom: public APacket
  {
  private:
  	static uint16_t headerNumber;

  public:
    JoinRoom();
    JoinRoom(uint16_t idRoom);

    uint16_t getRoomId() const;

  private:
	std::string to_bytes_body() const override;
    std::size_t from_bytes_body(const std::string &bytes) override;
	uint16_t getHeaderNumber() const override {return headerNumber;};

    uint16_t  _idRoom;
  };
};

#endif