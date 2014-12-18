#include "Panel.hpp"

Panel::Panel(const sf::FloatRect &rect, bool hide)
: ADrawable(hide, {rect.left, rect.top}, {rect.width, rect.height})
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
  if (!_hide)
  {
    sf::View  view({_pos.x, _pos.y, _size.x, _size.y});
    
    view.setViewport({_pos.x / win.getSize().x, _pos.y / win.getSize().y,
      _size.x / win.getSize().x, _size.y / win.getSize().y});
    win.setView(view);
    for (auto it : _display)
      if (!it->isHidden())
        it->draw(win);
    win.setView(win.getDefaultView());
  }
}

void  Panel::add(const std::shared_ptr<ADrawable> &ptr)
{
  _display.push_back(ptr);
}

