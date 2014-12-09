#ifndef _APANEL_HPP_
# define _APANEL_HPP_

# include <SFML/Graphics.hpp>
# include "Observer.hpp"

class APanel : public Observable
{
public:
  APanel(bool hide = true): _hide(hide) { };
  virtual ~APanel() {};

  virtual bool update(const sf::Event &) = 0;
  virtual void draw(sf::RenderWindow &) = 0;

  void  setHide(bool hide) { _hide = hide; };
  bool  isHidden() const { return _hide; };

private:
  bool  _hide;
};

#endif
