#include <Packet/StartGame.hpp>
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
};

Game::Game(const sf::FloatRect &rect)
: Panel(rect), _network(Network::NetworkFactory::createNetwork()), _begin(false), _scrollSpeed(2)
{
  auto background = RessourceManager::instance().getTexture("../assets/gameBackground.png");
  _background = std::shared_ptr<Image>(new Image(std::shared_ptr<sf::Sprite>(new sf::Sprite(*background)),
                                                 {0, 0, static_cast<float>(background->getSize().x), 900}));
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
  for (auto &it : _players)
    it.second->draw(win);
}

void  Game::update(const Input &event, float timeElapsed)
{
  bool move = false;
  
  if (isConnected())
    _network->poll();
  if (_begin)
  {
    if (event.getState("fire"))
      _players[_playerId]->fire();
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
    auto pos = sf::Vector2f(_background->getPosition().x - (_scrollSpeed * timeElapsed), _background->getPosition().y);
    if (pos.x <= -_background->getSize().x)
      pos.x = 0;
    _background->setPosition(pos);
  }
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

void  Game::createPlayer(uint16_t playerId)
{
  std::shared_ptr<sf::Texture> playerTexture(new sf::Texture);
  auto weaponTexture = RessourceManager::instance().getTexture("../assets/missile.png");

  if (!playerTexture->loadFromFile("../assets/spaceShip.gif", {0, playerId * 16, 166, 16}))
    throw std::runtime_error("../assets/spaceShip.gif cannot be found");

  std::shared_ptr<AnimatedSprites> sprite(new AnimatedSprites(sf::FloatRect(0, static_cast<float>(playerId * 32), 99, 48), 5, playerTexture->getSize().y, playerTexture));
  std::shared_ptr<AnimatedSprites> weaponSprite(new AnimatedSprites(sf::FloatRect(0, 0, 99, 48), 2, weaponTexture->getSize().y, weaponTexture));
  std::shared_ptr<AWeapon> weapon(new BasicWeapon(weaponSprite));
  std::shared_ptr<Player>  ptr(new Player({0, static_cast<float>(playerId * 48)}, {10, 10}, sprite, weapon));
  
  RessourceManager::instance().save(playerTexture);
  _players[playerId] = ptr;
}