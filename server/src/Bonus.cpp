#include "Bonus.hpp"

template<>
struct enum_traits<BonusTypes>
{
  static const BonusTypes enumerators[];
};

const BonusTypes enum_traits<BonusTypes>::enumerators[] = {
  BonusTypes::SPEED,
  BonusTypes::LIFE
};


Bonus::Bonus(int networkId, int xPos, BonusTypes bon)
  : AEntity(networkId, EntitiesTypes::MONSTER, xPos), _bonusType(bon)
{
}

