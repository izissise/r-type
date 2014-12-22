#ifndef _APANEL_HPP_
# define _APANEL_HPP_

# include <SFML/Graphics.hpp>
# include <memory>
# include "ADrawable.hpp"
# include "Observer.hpp"

class Panel : public ADrawable
{
public:
  
  enum class PanelId
  {
    MENUPANEL = 1,
    LISTPANEL = 2,
    CREATEROOMPANEL = 3,
    ROOMPANEL = 4,
    GAMEPANEL = 5,
  };
  
  Panel(const sf::FloatRect &rect, bool hide = false);
  virtual ~Panel() {};

  virtual void  update(const sf::Event &, float timeElapsed);
  virtual void  draw(sf::RenderWindow &);
  
  virtual void  add(const std::shared_ptr<ADrawable> &ptr);

private:
  std::vector<std::shared_ptr<ADrawable>> _display;
};

#endif
