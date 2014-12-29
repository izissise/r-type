#include <iostream>
#include "BasicWeapon.hpp"

BasicWeapon::BasicWeapon(const std::shared_ptr<AnimatedSprites> &sprite)
: AWeapon({0, 0}, {300, 300},  1, sprite)
{

}

BasicWeapon::~BasicWeapon()
{

}

void BasicWeapon::update(const Input & , float )
{
}

void BasicWeapon::draw(sf::RenderWindow &)
{
}