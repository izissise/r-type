#include "Monster.hpp"

const std::string& MegaBlorbMonster::getName() const
{
 return _name;
}

int MegaBlorbMonster::getPower() const
{
 return _power;
}

int MegaBlorbMonster::getLife() const
{
 return _life;
}

extern "C"
{

megaBlorbMonster_EXPORT ILibMonster* createMonster()
{
  return new MegaBlorbMonster();
}

}