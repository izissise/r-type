#ifndef _GAME_HPP_
# define _GAME_HPP_

# include <iostream>
# include <memory>
# include <SFML/Audio.hpp>

# include "Panel.hpp"
# include "Image.hpp"
# include "RtypeProtoHelper.hpp"
# include "NetworkFactory.hpp"
# include "Player.hpp"
# include "RessourceManager.hpp"
# include "AnimatedSprites.hpp"
# include "AWeapon.hpp"

# include "Packet/Handshake.hpp"
# include "Packet/ShortResponse.hpp"
# include "Packet/GetListPlayer.hpp"
# include "Packet/MovePacket.hpp"
# include "Packet/NewBonus.hpp"
# include "Packet/NewMonster.hpp"

struct t_netEntity
{
	t_netEntity(const Vector<float> &p, const Vector<float> &s, const std::shared_ptr<Image> &sp)
		: pos(p), speed(s), sprite(sp) {};
	Vector<float>	pos;
	Vector<float>	speed;
	std::shared_ptr<Image> sprite;
};

class Game : public Panel, public RtypeProtoHelper<Game>
{
  friend RtypeProtoHelper<Game>;

public:
  Game(const sf::FloatRect &rect);
  ~Game() = default;

  void  update(const Input &, float) override;
  void  draw(sf::RenderWindow &win) override;
  bool  connect(const std::string &ip, const std::string &port, const std::string &login, uint16_t playerId);

private:
  void  onDisconnet() override;

  std::size_t netShortResponse(const Network::Buffer &data);
  std::size_t netGetListPlayer(const Network::Buffer &data);
  std::size_t netStartGame(const Network::Buffer &data);
  std::size_t netMovePlayer(const Network::Buffer &data);
  std::size_t netMissile(const Network::Buffer &data);
  size_t  netNewBonus(const Network::Buffer &data);
  size_t  netNewMonster(const Network::Buffer &data);


  void  createPlayer(uint16_t playerId);
  void  createMissile(uint16_t playerId);

  std::unique_ptr<Network::ANetwork>  _network;
  bool                                _begin;
  uint16_t                            _playerId;
  std::map<uint16_t, std::shared_ptr<Player>>          _players;
  std::vector<t_netEntity> _monster;
  std::vector<t_netEntity> _bonus;
  float                               _scrollSpeed;
  std::shared_ptr<Image>              _background;
  sf::Music							  _music;
  sf::Music							  _fire;
};

#endif
