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

  private:
    std::string to_bytes_body() const override;
    void from_bytes_body(const std::string &bytes) override;

    std::vector<t_room>       _list;
  };
};

#endif