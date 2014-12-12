#include "Text.hpp"

Text::Text(const std::string &text)
: ADrawable(), _text(new sf::Text())
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

void  Text::setPosition(const sf::Vector2f &pos)
{
  _text->setPosition(pos);
}

void  Text::setFont(const sf::Font &font)
{
  _text->setFont(font);
}

const sf::FloatRect Text::getLocalBounds() const
{
  return (_text->getLocalBounds());
}


void Text::update(const sf::Event &)
{
  
}

void Text::draw(sf::RenderWindow &win)
{
  win.draw(*_text);
}
