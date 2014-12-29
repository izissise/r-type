#include "Text.hpp"

Text::Text(const sf::FloatRect &rect, const std::string &text)
: ADrawable(false, {rect.left, rect.top}, {rect.width, rect.height}), _text(new sf::Text())
{
  _text->setString(text);
}

Text::~Text()
{
  
}

const std::string Text::getString() const
{
  return (_text->getString());
}

void  Text::move(float x, float y)
{
  _text->move(x, y);
}

void  Text::setColor(const sf::Color &c) const
{
  _text->setColor(c);
}

void  Text::setCharacterSize(unsigned int size)
{
  _text->setCharacterSize(size);
}

void  Text::setString(const std::string &str)
{
  _text->setString(str);
}

void  Text::setFont(const sf::Font &font)
{
  _text->setFont(font);
}

const sf::FloatRect Text::getLocalBounds() const
{
  return (_text->getLocalBounds());
}


void Text::update(const Input &, float)
{
  
}

void Text::draw(sf::RenderWindow &win)
{
  _text->setPosition(_pos.x, _pos.y);
  win.draw(*_text);
}
