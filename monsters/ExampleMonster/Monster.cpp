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

exampleMonster_EXPORT ILibMonster* createMonster()
{
  return new ExampleMonster();
}

}