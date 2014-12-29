#include <iostream>
#include "Player.hpp"

Player::Player(const Vector<float> &pos, const Vector<float> &speed, const std::shared_ptr<AnimatedSprites> &sprites, const std::shared_ptr<AWeapon> &weapon)
: AEntity(pos, {sprites->getSize().x, sprites->getSize().y}, 1, 0, sprites), _speed(speed), _weapon(weapon)
{
  _sprite->setCurrentAnim(Player::Animation::NORMAL);
}

Player::~Player()
{
  
}

const Vector<float> &Player::getSpeed() const
{
  return _speed;
}

void  Player::setSpeed(const Vector<float> &speed)
{
  _speed = speed;
}

const std::shared_ptr<AWeapon> &Player::getWeapon() const
{
  return _weapon;
}

void Player::setWeapon(const std::shared_ptr<AWeapon> &weapon)
{
  _weapon = weapon;
}

void Player::move(uint8_t axis, float time)
{
  sf::Vector2f tmp = _pos;
  
  if (axis == 1)
    tmp.x += _speed.x * time;
  else
    tmp.y += _speed.y * time;
  if (!(tmp.x < 0 || tmp.x + _sprite->getSize().x > 1600 || tmp.y < 0 || tmp.y + _sprite->getSize().y > 900))
  {
    if (axis == 0)
    {
      if (tmp.y < _pos.y)
      {
        if (_sprite->getCurrentAnim() == Player::Animation::NORMAL)
          _sprite->setCurrentAnim(Player::Animation::MIDUP);
        else if (_sprite->getCurrentAnim() == Player::Animation::MIDUP)
          _sprite->setCurrentAnim(Player::Animation::UP);
      }
      else
      {
        if (_sprite->getCurrentAnim() == Player::Animation::NORMAL)
          _sprite->setCurrentAnim(Player::Animation::MIDDOWN);
        else if (_sprite->getCurrentAnim() == Player::Animation::MIDDOWN)
          _sprite->setCurrentAnim(Player::Animation::DOWN);
      }
    }
    _pos = tmp;
  }
}

void Player::fire()
{
  std::cout << "Fire" << std::endl;
  _ammo.push_back(_weapon->fire({_pos.x + _sprite->getSize().x, _pos.y}));
}

void Player::update(const Input &, float timeElaspsed)
{
  for (auto &it : _ammo) {
    it.pos.x += (it.speed.x * timeElaspsed);
  }
}

void Player::draw(sf::RenderWindow &win)
{
  for (auto &it : _ammo)
  {
    it.sprite->setPosition(sf::Vector2f(it.pos.x, it.pos.y));
    it.sprite->draw(win);
  }
  _sprite->setPosition(_pos);
  _sprite->draw(win);
}


