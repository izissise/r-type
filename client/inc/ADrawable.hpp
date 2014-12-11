#ifndef _ADRAWABLE_HPP_
# define _ADRAWABLE_HPP_

# include <SFML/Graphics.hpp>

class ADrawable
{
public:
  ADrawable(bool hide = false);
  virtual ~ADrawable() { };

  virtual void update(const sf::Event &) = 0;
  virtual void draw(sf::RenderWindow &win) = 0;
  
  void  setHide(bool hide);
  bool  isHidden() const;

protected:
  bool  _hide;
};

#endif
