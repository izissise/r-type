#ifndef NEWMONSTER_H
#define NEWMONSTER_H

#include "APacket.hpp"

namespace Packet {

class NewMonster : public APacket
{
public:
  NewMonster();
  NewMonster();

  virtual ~NewMonster() = default;

  std::string to_bytesNoHeader() const override;
  size_t from_bytes(const std::string &bytes) override;
protected:
};

};

#endif // NEWMONSTER_H
