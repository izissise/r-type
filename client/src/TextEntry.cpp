#include "TextEntry.hpp"

TextEntry::TextEntry(const std::string &placeHolder, const sf::Vector2f &pos, const std::shared_ptr<sf::Sprite> &back)
: _pos(pos), _use(false), _text(""), _first(true), _background(back),
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
  if (_first)
    _displayText.setString(_placeHolder);
  else
    _displayText.setString(_text);
  _background->setPosition(_pos);
  _displayText.setPosition(_pos);
  win.draw(*_background);
  win.draw(_displayText);
}

void  TextEntry::update(const sf::Event &event)
{
  
}
