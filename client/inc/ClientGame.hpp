#ifndef _CLIENTGAME_HPP_
# define _CLIENTGAME_HPP_

# include <SFML/Graphics.hpp>
# include <iostream>
# include <thread>
# include <memory>
# include "Observer.hpp"
# include "APanel.hpp"
# include "MenuPanel.hpp"

class ClientGame: public IObserver
{
public:
  ClientGame();
  ~ClientGame();

  void  run();
  bool  update();
  void  draw();
  virtual void	trigger(const t_event &event);
  
private:
  std::vector<std::shared_ptr<APanel>> _panel;
  sf::RenderWindow    _win;
  bool                _done;
};

#endif