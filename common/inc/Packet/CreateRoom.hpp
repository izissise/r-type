#ifndef _CREATEROOM_HPP_
# define _CREATEROOM_HPP_

# include <string>
# include <memory>
# include "APacket.hpp"
# include "Room.hpp"

namespace Packet {
  class CreateRoom: public APacket
  {
  public:
    CreateRoom();
    CreateRoom(const t_room &);

    virtual ~CreateRoom() = default;

    const std::shared_ptr<t_room>  &getRoom() const;

    std::string to_bytesNoHeader() const override;
    size_t from_bytes(const std::string &bytes) override;

  private:
    std::shared_ptr<t_room>  _room;
  };
};

#endif