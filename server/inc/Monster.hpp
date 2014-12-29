#ifndef MONSTER_H
#define MONSTER_H

#include "AEntity.hpp"
#include "ILibMonster.hpp"

class Monster : public AEntity
{
public:
  Monster(int networkId, int xPos, const std::shared_ptr<ILibMonster>& ms);
  virtual ~Monster() = default;

  const std::string& getName() const {return _ms->getName();};
  size_t getLife() const {return _ms->getLife();};

protected:
   std::shared_ptr<ILibMonster> _ms;
};

#endif // MONSTER_H
