#include "AnimatedSprites.hpp"

AnimatedSprites::AnimatedSprites(const sf::FloatRect &rect, uint8_t nbAnim, uint32_t heigth, const std::shared_ptr<sf::Texture> &texture)
: ADrawable(false, {rect.left, rect.top}, {rect.width, rect.height})
{
  uint64_t size = (texture->getSize().x / nbAnim);
  for (uint8_t i = 0;i < nbAnim && i * size < texture->getSize().x;++i)
  {
    std::shared_ptr<sf::Sprite> sprite(new sf::Sprite(*texture));
    
    sprite->setTextureRect(sf::IntRect(size * i, 0, size, heigth));
    _sprites.push_back(std::shared_ptr<Image>(new Image(sprite, {0, 0, 0, 0})));
  }
}

void AnimatedSprites::update(const sf::Event &, float)
{
  
}

void AnimatedSprites::draw(sf::RenderWindow &win)
{
  if (_currentAnim < _sprites.size())
  {
    _sprites[_currentAnim]->setPosition(_pos);
    _sprites[_currentAnim]->setSize(_size);
    _sprites[_currentAnim]->draw(win);
  }
}

void  AnimatedSprites::setCurrentAnim(uint8_t anim)
{
  _currentAnim = anim;
}
