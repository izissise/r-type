#ifndef _IMAGE_HPP_
# define _IMAGE_HPP_

# include <SFML/Graphics.hpp>
# include <memory>
# include "ADrawable.hpp"

class Image : public ADrawable
{
public:
  Image(const std::shared_ptr<sf::Sprite> &image);
  ~Image();
  
  virtual void update(const sf::Event &);
  virtual void draw(sf::RenderWindow &win);
  
  void  setPosition(const sf::Vector2f &pos);
  void  setScale(float x, float y);
  const sf::IntRect  &getTextureRect() const;

private:
  std::shared_ptr<sf::Sprite> _image;
};


#endif
