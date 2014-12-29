#ifndef _ENTITYDIE_HPP_
# define _ENTITYDIE_HPP_

# include "APacket.hpp"

namespace Packet {
  class EntityDie: public APacket
  {
  public:
    EntityDie();
    EntityDie(size_t entityId);

    virtual ~EntityDie() = default;

    size_t getId() const {return _entityId;};

    virtual std::string to_bytesNoHeader() const override;
    virtual size_t from_bytes(const std::string &bytes) override;

  private:
    uint16_t   _entityId;
  };
};

#endif