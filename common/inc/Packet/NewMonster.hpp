#ifndef NEWMONSTER_H
#define NEWMONSTER_H

#include <string>

#include "APacket.hpp"

namespace Packet {

class NewMonster : public APacket
{
public:
  NewMonster() = default;
  NewMonster(const std::string& name, int power, int life);

  virtual ~NewMonster() = default;

  std::string to_bytesNoHeader() const override;
  size_t from_bytes(const std::string &bytes) override;
protected:
	std::string _name;
	int			_power;
	int			_life;
};

};

#endif // NEWMONSTER_H
