#include "Monster.hpp"

const std::string& BlorbMonster::getName() const
{
 return _name;
}

int BlorbMonster::getPower() const
{
 return _power;
}

int BlorbMonster::getLife() const
{
 return _life;
}

extern "C"
{

blorbMonster_EXPORT ILibMonster* createMonster()
{
  return new BlorbMonster();
}

}