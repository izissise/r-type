#ifndef _GAME_HPP_
# define _GAME_HPP_

# include <iostream>

# include "Panel.hpp"
# include "RtypeProtoHelper.hpp"
# include "NetworkFactory.hpp"

# include "Handshake.hpp"

class Game : public Panel, public RtypeProtoHelper<Game>
{
  friend RtypeProtoHelper;

public:
  Game(const sf::FloatRect &rect);
  ~Game() = default;

  void  update(const sf::Event &) override;
  bool  connect(const std::string &ip, const std::string &port, const std::string &login);

private:
  void  onDisconnet() override;

  std::unique_ptr<Network::ANetwork>  _network;
};

#endif
