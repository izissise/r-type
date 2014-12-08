#include "MenuPanel.hpp"

MenuPanel::MenuPanel(bool hide)
: APanel(hide)
{
  
}

MenuPanel::~MenuPanel()
{
  
}

bool MenuPanel::update(const sf::Event &event)
{
  switch (event.type) {
    case sf::Event::Closed :
      return (false);
    default:
      break;
  }
  return (true);
}

void MenuPanel::draw(sf::RenderWindow &win)
{
  sf::CircleShape circle(100, 100);

  circle.setFillColor(sf::Color::Blue);
  win.draw(circle);
}
