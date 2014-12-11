#include "Image.hpp"

Image::Image(const std::shared_ptr<sf::Sprite> &image)
: _image(image)
{
  
}

Image::~Image()
{
  
}

void Image::update(const sf::Event &)
{
  
}

void Image::draw(sf::RenderWindow &win)
{
  win.draw(*_image);
}

const sf::IntRect  &Image::getTextureRect() const
{
  return (_image->getTextureRect());
}

void  Image::setScale(float x, float y)
{
  _image->setScale(x, y);
}

void  Image::setPosition(const sf::Vector2f &pos)
{
  _image->setPosition(pos.x, pos.y);
}
