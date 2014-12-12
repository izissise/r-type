#ifndef _JOINROOM_HPP_
# define _JOINROOM_HPP_

# include "APacket.hpp"

namespace Packet {
  class JoinRoom: public APacket
  {
  public:
    JoinRoom(uint16_t idRoom);
    
    virtual const std::string to_bytes_body() const;
    virtual void from_bytes_body(const std::string &bytes);
    
    uint16_t getRoomId() const;
  private:
    uint16_t  _idRoom;
  };
}

#endif