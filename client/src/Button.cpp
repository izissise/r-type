#include "Button.hpp"

Button::Button(const sf::Vector2f &pos,
               const std::shared_ptr<sf::Sprite> &displayTexture,
               const std::shared_ptr<sf::Sprite> &hoverTexture,
               const std::shared_ptr<sf::Sprite> &clickedTexture)
: _hover(false), _isClicked(false),
_pos(pos), _displayTexture(displayTexture), _hoverTexture(hoverTexture), _clickedTexture(clickedTexture)
{
  _displayTexture->setPosition(_pos);
  _hoverTexture->setPosition(_pos);
  _clickedTexture->setPosition(_pos);
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
        sf::FloatRect  rect = _displayTexture->getGlobalBounds();

        if (event.mouseMove.x >= _pos.x && event.mouseMove.x <= _pos.x + rect.width
            && event.mouseMove.y >= _pos.y && event.mouseMove.y <= _pos.y + rect.height)
          _hover = true;
      }
      else
      {
        sf::FloatRect  rect = _displayTexture->getGlobalBounds();

        if (!(event.mouseMove.x >= _pos.x && event.mouseMove.x <= _pos.x + rect.width
            && event.mouseMove.y >= _pos.y && event.mouseMove.y <= _pos.y + rect.height))
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
    win.draw(*_clickedTexture);
  else if (isHover())
    win.draw(*_hoverTexture);
  else
    win.draw(*_displayTexture);
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
