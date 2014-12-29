#include <sstream>
#include <Packet/StartGame.hpp>
#include <Packet/LaunchMissile.hpp>
#include "UDPConnectionProblem.hpp"
#include "BasicWeapon.hpp"
#include "Game.hpp"

template<>
std::map<Packet::APacket::PacketType, size_t (Game::*)(const Network::Buffer&)> RtypeProtoHelper<Game>::_netWorkBinds =
{
  { Packet::APacket::PacketType::SHORTRESPONSE, &Game::netShortResponse },
  { Packet::APacket::PacketType::GETLISTPLAYER, &Game::netGetListPlayer },
  { Packet::APacket::PacketType::STARTGAME, &Game::netStartGame },
  { Packet::APacket::PacketType::MOVE, &Game::netMovePlayer },
  { Packet::APacket::PacketType::NEWBONUS, &Game::netNewBonus },
  { Packet::APacket::PacketType::NEWMONSTER, &Game::netNewMonster },
  { Packet::APacket::PacketType::LAUNCHMISSILE, &Game::netMissile },

};

Game::Game(const sf::FloatRect &rect)
: Panel(rect), _network(Network::NetworkFactory::createNetwork()), _begin(false), _scrollSpeed(0.25)
{
  auto background = RessourceManager::instance().getTexture("../assets/gameBackground.png");
  _background = std::shared_ptr<Image>(new Image(std::shared_ptr<sf::Sprite>(new sf::Sprite(*background)),
                                                 {0, 0, static_cast<float>(background->getSize().x), 900}));
  _music.openFromFile("../assets/ingame.ogg");
  _fire.openFromFile("../assets/fire.ogg");
  _life = std::shared_ptr<Text>(new Text({ 0, 0, 100, 30 }, "Life: "));
  _score = std::shared_ptr<Text>(new Text({ 0, 30, 100, 30 }, "Score: "));

  _life->setCharacterSize(30);
  _score->setCharacterSize(30);
  _life->setFont(*RessourceManager::instance().getFont("../assets/font.ttf"));
  _score->setFont(*RessourceManager::instance().getFont("../assets/font.ttf"));
  _life->setColor(sf::Color::White);
  _score->setColor(sf::Color::White);
}

void  Game::draw(sf::RenderWindow &win)
{
  if (_background->getPosition().x + _background->getSize().x < 1600)
  {
    auto pos = _background->getPosition();
    
    _background->setPosition({pos.x + _background->getSize().x, pos.y});
    _background->draw(win);
    _background->setPosition(pos);
  }
  _background->draw(win);
  _life->draw(win);
  _score->draw(win);
  for (auto &it : _players)
    it.second->draw(win);
  for (auto &it : _monster)
  {
	  it.sprite->setPosition({ it.pos.x, it.pos.y });
	  it.sprite->draw(win);
  }
  for (auto &it : _bonus)
  {
	  it.sprite->setPosition({ it.pos.x, it.pos.y });
	  it.sprite->draw(win);
  }
}

void  Game::update(const Input &event, float timeElapsed)
{
  bool move = false;
  
  if (isConnected())
    _network->poll();
  if (_begin)
  {
	  if (event.getState("fire"))
	  {
		  try
		  {
			  _players[_playerId]->fire();
			  _fire.play();
			  _writeBuff.writeBuffer(Packet::LaunchMissile(_playerId));
		  }
		  catch (std::runtime_error &e)
		  {
			  std::cout << e.what() << std::endl;
		  }
	  }
    if (event.getState("up"))
    {
      move = true;
	  _players[_playerId]->move(0, -timeElapsed);
	  _writeBuff.writeBuffer(Packet::MovePacket(_playerId, 0, -timeElapsed).to_bytes());
    }
    if (event.getState("down"))
    {
      move = true;
	  _players[_playerId]->move(0, timeElapsed);
	  _writeBuff.writeBuffer(Packet::MovePacket(_playerId, 0, timeElapsed).to_bytes());
    }
    if (event.getState("left"))
    {
      move = true;
	  _players[_playerId]->move(1, -timeElapsed);
	  _writeBuff.writeBuffer(Packet::MovePacket(_playerId, 1, -timeElapsed).to_bytes());
    }
    if (event.getState("right"))
    {
      move = true;
	  _players[_playerId]->move(1, timeElapsed);
	  _writeBuff.writeBuffer(Packet::MovePacket(_playerId, 1, timeElapsed).to_bytes());
    }
    if (!move)
      _players[_playerId]->setAnim(Player::Animation::NORMAL);
    for (auto &it : _players)
      it.second->update(event, timeElapsed);
    auto pos = sf::Vector2f(_background->getPosition().x - (_scrollSpeed * timeElapsed), _background->getPosition().y);
    if (pos.x <= -_background->getSize().x)
      pos.x = 0;
    _background->setPosition(pos);
	for (auto &it : _monster)
		it.pos.x += (it.speed.x * timeElapsed);
	for (auto &it : _bonus)
		it.pos.x += (it.speed.x * timeElapsed);

	for (auto it = _players.begin(); it != _players.end(); it++)
	{
		for (auto it1 = (*it).second->getAmmo().begin(); it1 != (*it).second->getAmmo().end(); it1++)
		{
			sf::FloatRect rect = { (*it1).pos.x, (*it1).pos.y, (*it1).sprite->getSize().x, (*it1).sprite->getSize().y };

			_monster.erase(std::remove_if(_monster.begin(), _monster.end(),
				[rect, it](const t_netEntity &monster) -> bool {
				if (rect.intersects({ monster.pos.x, monster.pos.y, monster.sprite->getSize().x, monster.sprite->getSize().y }))
				{
					(*it).second->setScore((*it).second->getScore() + monster.value);
					return (true);
				}
				return (false);
			}), _monster.end());

			_bonus.erase(std::remove_if(_bonus.begin(), _bonus.end(),
				[rect](const t_netEntity &bonus) -> bool {
				return (rect.intersects({ bonus.pos.x, bonus.pos.y, bonus.sprite->getSize().x, bonus.sprite->getSize().y }));
			}), _bonus.end());
		}
		sf::FloatRect p = { (*it).second->getPosition().x, (*it).second->getPosition().y, (*it).second->getSize().x, (*it).second->getSize().y };

		_monster.erase(std::remove_if(_monster.begin(), _monster.end(),
			[p, it](const t_netEntity &monster) -> bool {
			if (p.intersects({ monster.pos.x, monster.pos.y, monster.sprite->getSize().x, monster.sprite->getSize().y }))
			{
				(*it).second->hurt(monster.value);
				return (true);
			}
			return (false);
		}), _monster.end());

		_bonus.erase(std::remove_if(_bonus.begin(), _bonus.end(),
			[p, it](const t_netEntity &bonus) -> bool {
			if (p.intersects({ bonus.pos.x, bonus.pos.y, bonus.sprite->getSize().x, bonus.sprite->getSize().y }))
			{
				(*it).second->setLife((*it).second->getLife() + bonus.value);
				return (true);
			}
			return (false);
		}), _bonus.end());
	}
  }
  std::stringstream ss("");
  ss << "Life: " << static_cast<int>(_players[_playerId]->getLife());
  _life->setString(ss.str());
  ss.str("");
  ss.clear();
  ss << "Score: " << _players[_playerId]->getScore();
  _score->setString(ss.str());
}

bool  Game::connect(const std::string &ip, const std::string &port, const std::string &login, uint16_t playerId)
{
  try {
    _playerId = playerId;
    std::shared_ptr<Network::ABasicSocket> socket = Network::NetworkFactory::createConnectSocket(ip, port, Network::ASocket::SockType::UDP);
    _network->registerClient(socket);
    setSocket(socket);
    _writeBuff.writeBuffer(Packet::Handshake(login, _playerId));
    _playerId = playerId;
    createPlayer(_playerId);
	_music.play();
	_music.setLoop(true);
    return true;
  }
  catch (Network::Error &e) {
    std::cerr << e.what() << std::endl;
    return false;
  }
}

void  Game::onDisconnet()
{
  std::cout << "UDP Disconnected" << std::endl;
}

std::size_t  Game::netShortResponse(const Network::Buffer &data)
{
  Packet::ShortResponse  rep;
  size_t  nbUsed;
  
  nbUsed = rep.from_bytes(data);
  if (rep.getResponse() == 1)
  {
    std::cout << "Connection Accepted" << std::endl;
  }
  else
    throw UDPConnectionProblem("Rejected By the server");
  return nbUsed;
}

std::size_t  Game::netGetListPlayer(const Network::Buffer &data)
{
  Packet::GetListPlayer  rep;
  size_t  nbUsed;
  
  nbUsed = rep.from_bytes(data);
  std::cout << "UDP Get Player = [";
  for (auto &it : rep.getPlayerList())
  {
    std::cout << it.name;
    try {
      _players.at(it.id);
    } catch (std::exception &e) {
      std::cout << "(create id = " << it.id << ")";
      createPlayer(it.id);
    }
    std::cout << ", ";
  }
  std::cout << "]" << std::endl;
  return nbUsed;
}

std::size_t  Game::netStartGame(const Network::Buffer &data)
{
  Packet::StartGame  rep;
  size_t  nbUsed;
  
  nbUsed = rep.from_bytes(data);
  _begin = true;
  return nbUsed;
}

std::size_t  Game::netMovePlayer(const Network::Buffer &data)
{
	Packet::MovePacket  rep;
	size_t  nbUsed;

	nbUsed = rep.from_bytes(data);
	_players[rep.getPlayerId()]->move(rep.getAxis(), rep.getSpeed());
	return nbUsed;
}

std::size_t  Game::netMissile(const Network::Buffer &data)
{
	Packet::LaunchMissile  m;
	size_t  nbUsed;

	nbUsed = m.from_bytes(data);
	try
	{
		_players.at(m.getPlayerId())->fire();
		_fire.play();
	}
	catch (std::runtime_error &e)
	{
		std::cout << e.what() << std::endl;
	}
	return nbUsed;
}

size_t Game::netNewBonus(const Network::Buffer& data)
{
	Packet::NewBonus    bn;
	size_t              nbUsed;
	int				  pos;
	int				  id;

	nbUsed = bn.from_bytes(data);
	id = bn.getId();
	pos = bn.getXPos();
	std::cout << "NewBonus" << std::endl;
	auto texture = RessourceManager::instance().getTexture("../assets/bonus.png");
	_bonus.push_back(t_netEntity(Vector<float>(1600, bn.getXPos() * 900 / 100), Vector<float>(-0.5, 1), 1,
	std::shared_ptr<Image>(new Image(std::shared_ptr<sf::Sprite>(new sf::Sprite(*texture)), { 0, 0, 30 * 2, 30 * 2 }))));
	return nbUsed;
}

size_t Game::netNewMonster(const Network::Buffer& data)
{
	Packet::NewMonster  mt;
	size_t              nbUsed;
	int				  pos;
	int				  id;
	std::string 		  name;

	nbUsed = mt.from_bytes(data);
	id = mt.getId();
	pos = mt.getXPos();
	name = mt.getName();
	std::cout << "NewMonster" << std::endl;

	auto texture = RessourceManager::instance().getTexture("../assets/" + name + ".png");
	_monster.push_back(t_netEntity(Vector<float>(1600, mt.getXPos() * 900 / 100), Vector<float>(-0.5, 1 ), mt.getLife(),
	std::shared_ptr<Image>( new Image(std::shared_ptr<sf::Sprite>(new sf::Sprite(*texture)), { 0, 0, static_cast<float>(texture->getSize().x) * 2, static_cast<float>(texture->getSize().y) * 2 }) )));
	return nbUsed;
}

void  Game::createPlayer(uint16_t playerId)
{
  std::shared_ptr<sf::Texture> playerTexture(new sf::Texture);
  auto weaponTexture = RessourceManager::instance().getTexture("../assets/missile.png");

  if (!playerTexture->loadFromFile("../assets/spaceShip.gif", {0, playerId * 17, 166, 16}))
    throw std::runtime_error("../assets/spaceShip.gif cannot be found");

  std::shared_ptr<AnimatedSprites> sprite(new AnimatedSprites(sf::FloatRect(0, static_cast<float>(playerId * 32), 99, 48), 5, playerTexture->getSize().y, playerTexture));
  std::shared_ptr<AnimatedSprites> weaponSprite(new AnimatedSprites(sf::FloatRect(0, 0, 99, 48), 2, weaponTexture->getSize().y, weaponTexture));
  std::shared_ptr<AWeapon> weapon(new BasicWeapon(weaponSprite));
  std::shared_ptr<Player>  ptr(new Player({0, static_cast<float>(playerId * 48)}, {1, 1}, sprite, weapon));
  
  RessourceManager::instance().save(playerTexture);
  _players[playerId] = ptr;
}