#include "ListPanel.hpp"

ListPanel::ListPanel(std::vector<t_room> &vec)
: _list(vec)
{
  
}

ListPanel::~ListPanel()
{
  
}

void  ListPanel::update(const sf::Event &event)
{
  Panel::update(event);
}

void  ListPanel::draw(sf::RenderWindow &win)
{
  Panel::draw(win);
}