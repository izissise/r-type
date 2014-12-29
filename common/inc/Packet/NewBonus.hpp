#ifndef _NEWBONUS_HPP_
# define _NEWBONUS_HPP_

# include "APacket.hpp"
# include "BonusTypes.hpp"

namespace Packet {
  class NewBonus: public APacket
  {
  public:
    NewBonus();
    NewBonus(size_t entityId, uint16_t xpos, BonusTypes bt);

    virtual ~NewBonus() = default;

    size_t getId() const {return _entityId;};
    BonusTypes getBonusType() const {return _bt;};
    uint16_t getXPos() const {return _xPos;};

    virtual std::string to_bytesNoHeader() const override;
    virtual size_t from_bytes(const std::string &bytes) override;

  private:
    uint16_t   _entityId;
    uint16_t   _xPos;
    BonusTypes _bt;
  };
};

#endif