#ifndef _IMAGE_HPP_
# define _IMAGE_HPP_

# include <SFML/Graphics.hpp>
# include <memory>
# include "ADrawable.hpp"

class Image : public ADrawable
{
public:
  Image(const std::shared_ptr<sf::Sprite> &image, const sf::FloatRect &pos);
  ~Image();
  
  virtual void update(const sf::Event &, float );
  virtual void draw(sf::RenderWindow &win);
  
private:
  std::shared_ptr<sf::Sprite> _image;
};


#endif
