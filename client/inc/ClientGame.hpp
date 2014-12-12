#ifndef _CLIENTGAME_HPP_
# define _CLIENTGAME_HPP_

# include <SFML/Graphics.hpp>
# include <algorithm>
# include <iostream>
# include <thread>
# include <memory>
# include "Observer.hpp"
# include "Panel.hpp"
# include "TextEntry.hpp"
# include "Button.hpp"
# include "Image.hpp"
# include "RessourceManager.hpp"
# include "NetworkFactory.hpp"

# define DEFAULTPORT "8000"

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
  void  createMenuPanel();
  
  std::vector<std::shared_ptr<Panel>> _panel;
  sf::RenderWindow                    _win;
  bool                                _done;
  
  std::unique_ptr<Network::ABasicSocket>  _socket;
};

#endif