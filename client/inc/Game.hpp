#ifndef _GAME_HPP_
# define _GAME_HPP_

# include <iostream>
# include "Panel.hpp"
# include "ClientHelper.hpp"
# include "NetworkFactory.hpp"

# include "Handshake.hpp"

class Game : public Panel, public Network::SocketClientHelper
{
public:
  Game(const sf::FloatRect &rect);
  ~Game();

  virtual void  update(const sf::Event &) override;
  bool  connect(const std::string &ip, const std::string &port, const std::string &login);
  
private:
  void  onRead(size_t sizeRead) override;
  void  onWrite(size_t sizeWrite) override;
  void  onDisconnet() override;

  std::unique_ptr<Network::ANetwork>  _network;
};

#endif