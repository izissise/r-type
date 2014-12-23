#ifndef _GAME_HPP_
# define _GAME_HPP_

# include <iostream>
# include <memory>

# include "Panel.hpp"
# include "Image.hpp"
# include "RtypeProtoHelper.hpp"
# include "NetworkFactory.hpp"
# include "Player.hpp"
# include "RessourceManager.hpp"

# include "Packet/Handshake.hpp"
# include "Packet/ShortResponse.hpp"

class Game : public Panel, public RtypeProtoHelper<Game>
{
  friend RtypeProtoHelper<Game>;

public:
  Game(const sf::FloatRect &rect);
  ~Game() = default;

  void  update(float);
  void  update(const sf::Event &, float) override;
  void  draw(sf::RenderWindow &win) override;
  bool  connect(const std::string &ip, const std::string &port, const std::string &login);

private:
  void  onDisconnet() override;

  std::size_t netShortResponse(const Network::Buffer &data);
  void  movePlayer(int axis, float speed);

  std::unique_ptr<Network::ANetwork>  _network;
  bool                                _begin;
  Player                              _player;
  float                               _scrollSpeed;
  std::shared_ptr<Image>              _image;
  std::shared_ptr<Image>              _background;
};

#endif
