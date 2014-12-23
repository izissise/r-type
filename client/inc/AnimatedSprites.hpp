#ifndef _ANIMATEDSPRITES_HPP_
# define _ANIMATEDSPRITES_HPP_

# include "ADrawable.hpp"
# include "Image.hpp"

class AnimatedSprites : public ADrawable
{
public:
  AnimatedSprites(const sf::FloatRect &rect, uint8_t nbAnim, uint32_t heigth, const std::shared_ptr<sf::Texture> &texture);
  
  virtual void update(const sf::Event &event, float) override;
  virtual void draw(sf::RenderWindow &) override;
  
  void  setCurrentAnim(uint8_t anim);
  
private:
  uint8_t                             _currentAnim;
  std::vector<std::shared_ptr<Image>> _sprites;
};

#endif
