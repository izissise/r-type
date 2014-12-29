#ifndef _NEWMONSTER_HPP_
# define _NEWMONSTER_HPP_

# include <string>

# include "APacket.hpp"

namespace Packet {
  class NewMonster : public APacket
  {
  public:
    NewMonster();
    NewMonster(size_t id, int xpos, const std::string& name, int life);

    virtual ~NewMonster() = default;

    const std::string &getName() const {return _name;};
    int getLife() const {return _life;};
    uint16_t getId() const {return _Id;};

    std::string to_bytesNoHeader() const override;
    size_t from_bytes(const std::string &bytes) override;

  private:
    std::string   _name;
    uint16_t	  _life;
    uint16_t	  _Id;
    uint16_t	  _xpos;
  };
};

#endif
