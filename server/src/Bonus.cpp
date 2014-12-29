#include "Bonus.hpp"

Bonus::Bonus(int networkId, int xPos, BonusTypes bon)
  : AEntity(networkId, EntitiesTypes::MONSTER, xPos), _bonusType(bon)
{
}

