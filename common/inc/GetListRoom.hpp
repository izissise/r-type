#ifndef _GETLISTROOM_HPP_
# define _GETLISTROOM_HPP_

# include <string>
# include <vector>
# include "APacket.hpp"
# include "Room.hpp"

namespace Packet {
  class GetListRoom: public APacket
  {
  public:
    GetListRoom();
    GetListRoom(const std::vector<t_room> &);

    const std::vector<t_room>  &getListRoom() const;

    std::string to_bytes() const override;
    size_t from_bytes(const std::string &bytes) override;

  private:
    std::vector<t_room>       _list;
  };
};

#endif