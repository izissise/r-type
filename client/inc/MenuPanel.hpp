#ifndef _MENUPANEL_HPP_
# define _MENUPANEL_HPP_

# include "APanel.hpp"

class MenuPanel : public APanel
{
public:
  MenuPanel(bool hide = true);
  ~MenuPanel();
  
  virtual bool update(const sf::Event &event);
  virtual void draw(sf::RenderWindow &win);
private:
};

#endif
