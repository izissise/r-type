#ifndef BONUS_H
#define BONUS_H

#include "AEntity.hpp"
#include "BonusTypes.hpp"

class Bonus : public AEntity
{
public:
  Bonus(int networkId, int xPos, BonusTypes bon);
  virtual ~Bonus() = default;

  BonusTypes getBonusType() const {return _bonusType;};
protected:
  BonusTypes _bonusType;
};

#endif // BONUS_H
