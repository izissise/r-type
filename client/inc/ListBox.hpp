#ifndef _LISTPANEL_HPP_
# define _LISTPANEL_HPP_

# include "ADrawable.hpp"
# include "Room.hpp"
# include "ListItem.hpp"
# include "RessourceManager.hpp"

class ListBox: public ADrawable
{
public:
  ListBox(const sf::FloatRect &pos, std::vector<t_room> &vec);
  ~ListBox();

  virtual void  update(const sf::Event &);
  virtual void  draw(sf::RenderWindow &);

private:
  void  updateEntry();
  
  sf::FloatRect           _pos;
  std::map<uint32_t, std::shared_ptr<ListItem>>   _items;
  std::vector<t_room>     _displayRoom;
  std::vector<t_room>     &_list;
};

#endif
