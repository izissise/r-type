#include "Bonus.hpp"

Bonus::Bonus(int networkId, int xPos, BonusTypes bon)
  : AEntity(networkId, EntitiesTypes::BONUS, xPos), _bonusType(bon)
{
}

