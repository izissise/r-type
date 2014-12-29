#include "Button.hpp"

Button::Button(const sf::FloatRect &pos,
               const std::shared_ptr<sf::Sprite> &displayTexture,
               const std::shared_ptr<sf::Sprite> &hoverTexture,
               const std::shared_ptr<sf::Sprite> &clickedTexture,
               const std::shared_ptr<Text> &text)
: ADrawable(false, {pos.left, pos.top}, {pos.width, pos.height}), _hover(false), _isClicked(false),
_hoverTexture(new Image(hoverTexture, pos)), _displayTexture(new Image(displayTexture, pos)),
_clickedTexture(new Image(clickedTexture, pos)), _text(text)
{
}

Button::Button(const sf::FloatRect &pos,
               const std::shared_ptr<sf::Sprite> &displayTexture,
               const std::shared_ptr<sf::Sprite> &hoverTexture,
               const std::shared_ptr<sf::Sprite> &clickedTexture)
: ADrawable(false, {pos.left, pos.top}, {pos.width, pos.height}), _hover(false), _isClicked(false),
_hoverTexture(new Image(hoverTexture, pos)), _displayTexture(new Image(displayTexture, pos)),
_clickedTexture(new Image(clickedTexture, pos)), _text(nullptr)
{
}

Button::~Button()
{
  
}

void  Button::update(const Input &event, float)
{
  auto mousePos = event.getMousePos();

  if (mousePos.x >= _pos.x && mousePos.x < _pos.x + _size.x
          && mousePos.y >= _pos.y && mousePos.y < _pos.y + _size.y)
    _hover = true;
  else if (!(mousePos.x >= _pos.x && mousePos.x < _pos.x + _size.x
          && mousePos.y >= _pos.y && mousePos.y < _pos.y + _size.y))
    _hover = false;
  if (event.isButtonPressed() && isHover())
    _isClicked = true;
  if (!event.isButtonPressed() && isClicked() && isHover())
  {
    if (_onClick)
      _onClick();
    _isClicked = false;
  }
}

void  Button::draw(sf::RenderWindow &win)
{
  sf::View  text({_pos.x, _pos.y, _size.x, _size.y});
  
  text.setViewport(sf::FloatRect(_pos.x / win.getSize().x, _pos.y / win.getSize().y, _size.x / win.getSize().x, _size.y / win.getSize().y));
  win.setView(text);
  if (isClicked())
  {
    auto size = _clickedTexture->getSize();
    _clickedTexture->setSize(size);
    _clickedTexture->setPosition({_pos.x, _pos.y});
    _clickedTexture->draw(win);
  }
  else if (isHover())
  {
    auto size = _hoverTexture->getSize();
    _hoverTexture->setSize(size);
    _hoverTexture->setPosition({_pos.x, _pos.y});
    _hoverTexture->draw(win);
  }
  else
  {
    auto size = _displayTexture->getSize();
    _displayTexture->setSize(size);
    _displayTexture->setPosition({_pos.x, _pos.y});
    _displayTexture->draw(win);
  }
  if (_text)
  {
    _text->setPosition({_pos.x + (_size.x / 2) - (_text->getLocalBounds().width / 2), _pos.y + (_size.y / 2) - (_text->getLocalBounds().height / 2)});
    _text->draw(win);
  }
  win.setView(win.getDefaultView());
}

bool  Button::isHover() const
{
  return _hover;
}

bool  Button::isClicked() const
{
  return _isClicked;
}

void  Button::onClick(const std::function<void ()> &func)
{
  _onClick = func;
}

