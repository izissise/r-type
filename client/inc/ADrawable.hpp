#ifndef _ADRAWABLE_HPP_
# define _ADRAWABLE_HPP_

# include <SFML/Graphics.hpp>

class ADrawable
{
public:
  ADrawable(bool hide, const sf::Vector2f &pos, const sf::Vector2f &size);
  virtual ~ADrawable() { };

  virtual void update(const sf::Event &, float) = 0;
  virtual void draw(sf::RenderWindow &win) = 0;
  
  void  setHide(bool hide);
  bool  isHidden() const;
  
  void  setSize(const sf::Vector2f &size);
  const sf::Vector2f &getSize() const;

  void  setPosition(const sf::Vector2f &pos);
  const sf::Vector2f &getPosition() const;

protected:
  bool          _hide;
  sf::Vector2f  _pos;
  sf::Vector2f  _size;
};

#endif
