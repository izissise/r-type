#ifndef _MENUPANEL_HPP_
# define _MENUPANEL_HPP_

# include <iostream>
# include <list>

# include "RessourceManager.hpp"
# include "APanel.hpp"
# include "Button.hpp"
# include "TextEntry.hpp"

class MenuPanel : public APanel
{
public:
  MenuPanel(bool hide = true);
  ~MenuPanel();
  
  virtual bool update(const sf::Event &event);
  virtual void draw(sf::RenderWindow &win);
private:
  sf::Sprite              _background;
  
  std::shared_ptr<TextEntry>  _loginEntry;
  std::shared_ptr<Button>     _connect;
  std::shared_ptr<Button>     _setting;
  std::shared_ptr<Button>     _exit;
  
  std::list<std::shared_ptr<sf::Texture>>  _text;
};

#endif
