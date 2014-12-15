#ifndef _LISTPANEL_HPP_
# define _LISTPANEL_HPP_

# include "Panel.hpp"
# include "Room.hpp"
# include "ListItem.hpp"

class ListPanel: public Panel
{
public:
  ListPanel(std::vector<t_room> &vec);
  ~ListPanel();

  virtual void  update(const sf::Event &);
  virtual void  draw(sf::RenderWindow &);
private:
  std::vector<ListItem>   _items;
  std::vector<t_room>     &_list;
};

#endif
