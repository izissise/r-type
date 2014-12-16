#include "TextEntry.hpp"

TextEntry::TextEntry(const std::string &placeHolder, const sf::FloatRect &pos, const std::shared_ptr<sf::Sprite> &back)
: ADrawable(), _pos(pos), _use(false), _text(""), _first(true), _isHover(false), _background(new Image(back)),
  _displayText(), _placeHolder(placeHolder)
{
}

TextEntry::~TextEntry()
{

}

void  TextEntry::setFont(const sf::Font &font)
{
  _displayText.setFont(font);
}

void  TextEntry::setCharacterSize(unsigned int size)
{
  _displayText.setCharacterSize(size);
}

void  TextEntry::setTextColor(const sf::Color &color)
{
  _displayText.setColor(color);
}

const std::string &TextEntry::getText() const
{
  return (_text);
}

TextEntry &TextEntry::operator+=(char c)
{
  _text += c;
  return (*this);
}

TextEntry &TextEntry::operator+=(const std::string &str)
{
  _text += str;
  return (*this);
}

void  TextEntry::draw(sf::RenderWindow &win)
{
  sf::View  text(_pos);

  text.setViewport(sf::FloatRect(_pos.left / win.getSize().x, _pos.top / win.getSize().y, _pos.width / win.getSize().x, _pos.height / win.getSize().y));
  win.setView(text);
  auto size = _background->getTextureRect();
  _background->setScale(_pos.width / size.width, _pos.height / size.height);
  _background->setPosition({_pos.left, _pos.top});
  if (_first)
    _displayText.setString(_placeHolder);
  else
    _displayText.setString(_text);
  if (_displayText.getLocalBounds().width > _pos.width)
    _displayText.setPosition({_pos.left + (_pos.width - _displayText.getLocalBounds().width),
                              _pos.top + (_pos.height / 2) - (_displayText.getLocalBounds().height / 2)});
  else
    _displayText.setPosition({_pos.left + (_pos.width / 2) - (_displayText.getLocalBounds().width / 2),
                              _pos.top + (_pos.height / 2) - (_displayText.getLocalBounds().height / 2)});
  _background->draw(win);
  _displayText.draw(win);
  win.setView(win.getDefaultView());
}

void  TextEntry::update(const sf::Event &event)
{
  if (event.type == sf::Event::MouseMoved)
  {
    if (event.mouseMove.x >= _pos.left && event.mouseMove.x < _pos.left + _pos.width
        && event.mouseMove.y >= _pos.top && event.mouseMove.y < _pos.top + _pos.height)
      _isHover = true;
    else
      _isHover = false;
  }
  if (_isHover && event.type == sf::Event::MouseButtonReleased)
  {
    _use = true;
    _first = false;
  }
  else if (!_isHover && event.type == sf::Event::MouseButtonReleased)
  {
    _use = false;
    if (_text.empty())
      _first = true;
  }
  if (_use && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::BackSpace && !_text.empty())
      _text.pop_back();
  if (_use && event.type == sf::Event::TextEntered && event.text.unicode > 20 && event.text.unicode < 128)
    _text += static_cast<char>(event.text.unicode);
}
