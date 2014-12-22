#include "Player.hpp"

Player::Player(const Vector<float> &pos, const Vector<float> &speed)
: AEntity(pos), _speed(speed)
{
  
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

uint8_t Player::getLife() const
{
  return _life;
}

void  Player::setLife(uint8_t life)
{
  _life = life;
}

uint16_t  Player::getScore() const
{
  return _score;
}

void  Player::setScore(uint16_t score)
{
  _score = score;
}
