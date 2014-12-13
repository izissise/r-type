#ifndef _GETLISTROOM_HPP_
# define _GETLISTROOM_HPP_

# include <string>
# include <vector>
# include "APAcket.hpp"
# include "Room.hpp"

namespace Packet {
  class GetListRoom: public APacket
  {
  public:
    GetListRoom();
    GetListRoom(const std::vector<t_room> &);
    
    const std::vector<t_room>  &getListRoom() const;
    
  private:
    virtual const std::string to_bytes_body() const;
    virtual void from_bytes_body(const std::string &bytes);
    
    std::vector<t_room>       _list;
  };
};

#endif