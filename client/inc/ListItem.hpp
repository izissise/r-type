#ifndef _LISTITEM_HPP_
# define _LISTITEM_HPP_

# include "ADrawable.hpp"
# include "Text.hpp"
# include "Button.hpp"

class ListItem: public ADrawable
{
public:
  ListItem(const sf::Vector2f &size, const std::shared_ptr<Text> &name, const std::shared_ptr<Text> player, const std::shared_ptr<Button> &entry);
  ~ListItem();
  
  virtual void update(const sf::Event &, float timeElapsed);
  virtual void draw(sf::RenderWindow &win);

private:
  sf::Vector2f                   _size;
  std::shared_ptr<Text>          _name;
  std::shared_ptr<Text>          _player;
  std::shared_ptr<Button>        _entry;
};

#endif