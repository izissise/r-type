#include "Monster.hpp"

const std::string& ExampleMonster::getName() const
{
 return _name;
}

int ExampleMonster::getPower() const
{
 return _power;
}

int ExampleMonster::getLife() const
{
 return _life;
}

extern "C"
{

DLLAPI AMonster* createMonster()
{
  return new ExampleMonster();
}

}