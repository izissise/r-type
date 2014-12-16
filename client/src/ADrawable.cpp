#include "ADrawable.hpp"

ADrawable::ADrawable(bool hide, const sf::Vector2f &pos, const sf::Vector2f &size)
: _hide(hide), _pos(pos), _size(size)
{
  
}

void  ADrawable::setHide(bool hide)
{
  _hide = hide;
}

bool  ADrawable::isHidden() const
{
  return _hide;
}

void  ADrawable::setSize(const sf::Vector2f &size)
{
  _size = size;
}

const sf::Vector2f &ADrawable::getSize() const
{
  return (_size);
}

void  ADrawable::setPosition(const sf::Vector2f &pos)
{
  _pos = pos;
}

const sf::Vector2f &ADrawable::getPosition() const
{
  return (_pos);
}