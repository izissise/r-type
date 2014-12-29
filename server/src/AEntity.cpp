#include "AEntity.hpp"

template<>
struct enum_traits<EntitiesTypes>
{
  static const EntitiesTypes enumerators[];
};

const EntitiesTypes enum_traits<EntitiesTypes>::enumerators[] = {
  EntitiesTypes::PLAYER,
  EntitiesTypes::MONSTER,
  EntitiesTypes::BONUS,
};

AEntity::AEntity(int networkId, EntitiesTypes type, int xPos)
  : _x(xPos), _networkId(networkId), _type(type)
{
}

