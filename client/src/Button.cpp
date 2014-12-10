#include "Button.hpp"

Button::Button(const sf::FloatRect &pos,
               const std::shared_ptr<sf::Sprite> &displayTexture,
               const std::shared_ptr<sf::Sprite> &hoverTexture,
               const std::shared_ptr<sf::Sprite> &clickedTexture)
: _hover(false), _isClicked(false),
_pos(pos), _displayTexture(displayTexture), _hoverTexture(hoverTexture), _clickedTexture(clickedTexture)
{
}

Button::~Button()
{
  
}

void  Button::update(const sf::Event &event)
{
  switch (event.type) {
    case sf::Event::MouseButtonPressed:
      if (isHover())
        _isClicked = true;
      break;
    case sf::Event::MouseButtonReleased:
      if (isClicked())
        _isClicked = false;
      break;
    case sf::Event::MouseMoved:
      if (!isHover())
      {
        if (event.mouseMove.x >= _pos.left && event.mouseMove.x <= _pos.left + _pos.width
            && event.mouseMove.y >= _pos.top && event.mouseMove.y <= _pos.top + _pos.height)
          _hover = true;
      }
      else
      {
        if (!(event.mouseMove.x >= _pos.left && event.mouseMove.x <= _pos.left + _pos.width
            && event.mouseMove.y >= _pos.top && event.mouseMove.y <= _pos.top + _pos.height))
          _hover = false;
      }
      break;
    default:
      break;
  }
}

void  Button::draw(sf::RenderWindow &win)
{
  if (isClicked())
  {
    auto size = _clickedTexture->getTextureRect();
    _clickedTexture->setScale(_pos.width / size.width, _pos.height / size.height);
    _clickedTexture->setPosition({_pos.left, _pos.top});
    win.draw(*_clickedTexture);
  }
  else if (isHover())
  {
    auto size = _hoverTexture->getTextureRect();
    _hoverTexture->setScale(_pos.width / size.width, _pos.height / size.height);
    _hoverTexture->setPosition({_pos.left, _pos.top});
    win.draw(*_hoverTexture);
  }
  else
  {
    auto size = _displayTexture->getTextureRect();
    _displayTexture->setScale(_pos.width / size.width, _pos.height / size.height);
    _displayTexture->setPosition({_pos.left, _pos.top});
    win.draw(*_displayTexture);
  }
}

bool  Button::isHidden() const
{
  return _hide;
}

bool  Button::isHover() const
{
  return _hover;
}

bool  Button::isClicked() const
{
  return _isClicked;
}
