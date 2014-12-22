#include "Image.hpp"

Image::Image(const std::shared_ptr<sf::Sprite> &image, const sf::FloatRect &pos)
: ADrawable(false, {pos.left, pos.top}, {pos.width, pos.height}), _image(image)
{
  
}

Image::~Image()
{
  
}

void Image::update(const sf::Event &, float )
{
  
}

void Image::draw(sf::RenderWindow &win)
{
  sf::IntRect size = _image->getTextureRect();
  
  _image->setScale(_size.x / (float)size.width, _size.y / (float)size.height);
  _image->setPosition(_pos.x, _pos.y);

  win.draw(*_image);
}
