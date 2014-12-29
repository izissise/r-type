#include "Monster.hpp"

Monster::Monster(int networkId, int xPos, const std::shared_ptr<ILibMonster>& ms)
  : AEntity(networkId, EntitiesTypes::MONSTER, xPos), _ms(ms)
{
}

