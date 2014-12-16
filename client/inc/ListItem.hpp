#ifndef _LISTITEM_HPP_
# define _LISTITEM_HPP_

# include "ADrawable.hpp"
# include "Text.hpp"
# include "Button.hpp"

class ListItem: public ADrawable
{
public:
  ListItem(const sf::Vector2f &size, const Text &name, const Text player, const Button &entry);
  ~ListItem();
  
  virtual void update(const sf::Event &);
  virtual void draw(sf::RenderWindow &win);

private:
  sf::Vector2f  _size;
  Text          _name;
  Text          _player;
  Button        _entry;
};

#endif