#include "Panel.hpp"

Panel::Panel(bool hide)
: ADrawable(hide)
{
  
}

void Panel::update(const sf::Event &event)
{
  for (auto it : _display)
    if (!it->isHidden())
      it->update(event);
}

void Panel::draw(sf::RenderWindow &win)
{
  for (auto it : _display)
    if (!it->isHidden())
      it->draw(win);
}

void  Panel::add(const std::shared_ptr<ADrawable> &ptr)
{
  _display.push_back(ptr);
}

