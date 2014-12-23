#include "UDPConnectionProblem.hpp"
#include "Game.hpp"

template<>
std::map<Packet::APacket::PacketType, size_t (Game::*)(const Network::Buffer&)> RtypeProtoHelper<Game>::_netWorkBinds =
{
  { Packet::APacket::PacketType::SHORTRESPONSE, &Game::netShortResponse },
  { Packet::APacket::PacketType::GETLISTPLAYER, &Game::netGetListPlayer },
};

Game::Game(const sf::FloatRect &rect)
: Panel(rect), _network(Network::NetworkFactory::createNetwork()), _begin(false), _scrollSpeed(5)
{
  auto background = RessourceManager::instance().getTexture("../assets/gameBackground.png");
  _background = std::shared_ptr<Image>(new Image(std::shared_ptr<sf::Sprite>(new sf::Sprite(*background)),
                                                {0, 0, static_cast<float>(background->getSize().x), 900}));
}

void  Game::movePlayer(int axis, float speed, uint16_t playerId)
{
  auto img = _players[playerId]->image;
  auto pos = img->getPosition();

  if (axis == 1)
    pos.x += speed;
  else
    pos.y += speed;
  if (!(pos.x < 0 || pos.x + img->getSize().x > 1600 || pos.y < 0 || pos.y + img->getSize().y > 900))
    img->setPosition(pos);
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
    it.second->image->draw(win);
}

void  Game::update(float timeElapsed)
{
  auto pos = sf::Vector2f(_background->getPosition().x - (_scrollSpeed * timeElapsed), _background->getPosition().y);
  if (pos.x <= -_background->getSize().x)
    pos.x = 0;
  _background->setPosition(pos);
}

void  Game::update(const sf::Event &event, float timeElapsed)
{
  float x = sf::Joystick::getAxisPosition(0, sf::Joystick::X) / 100;
  float y = sf::Joystick::getAxisPosition(0, sf::Joystick::Y) / 100;

  if (isConnected())
    _network->poll();
  if (y != 0)
    movePlayer(0, _players[_playerId]->player.getSpeed().y * y * timeElapsed, _playerId);
  if (x != 0)
    movePlayer(1, _players[_playerId]->player.getSpeed().x * x * timeElapsed, _playerId);
  if ((event.type == sf::Event::JoystickButtonPressed && event.joystickButton.button == 11)
       || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space))
    std::cout << "Fire" << std::endl;
  if (event.type == sf::Event::KeyPressed)
  {
    switch (event.key.code)
    {
      case sf::Keyboard::Up :
        movePlayer(0, -_players[_playerId]->player.getSpeed().y * timeElapsed, _playerId);
        break;
      case sf::Keyboard::Down :
        movePlayer(0, _players[_playerId]->player.getSpeed().y * timeElapsed, _playerId);
        break;
      case sf::Keyboard::Left :
        movePlayer(1, -_players[_playerId]->player.getSpeed().x * timeElapsed, _playerId);
        break;
      case sf::Keyboard::Right :
        movePlayer(1, _players[_playerId]->player.getSpeed().x * timeElapsed, _playerId);
        break;
      default:
        break;
    }
  }
}

bool  Game::connect(const std::string &ip, const std::string &port, const std::string &login, uint16_t playerId)
{
  try {
    std::shared_ptr<Network::ABasicSocket> socket = Network::NetworkFactory::createConnectSocket(ip, port, Network::ASocket::SockType::UDP);
    _network->registerClient(socket);
    setSocket(socket);
    _writeBuff.writeBuffer(Packet::Handshake(login, _playerId).to_bytes());
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
    _begin = true;
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
    std::cout << it.name << ",";
    try {
      _players.at(it.id);
    } catch (std::exception &e) {
      createPlayer(it.id);
    }
  }
  std::cout << "]" << std::endl;
  return nbUsed;
}

void  Game::createPlayer(uint16_t playerId)
{
  std::shared_ptr<sf::Texture> playerTexture(new sf::Texture);
  
  if (!playerTexture->loadFromFile("../assets/spaceShip.gif", {0, playerId * 16, 166, 16}))
    throw std::runtime_error("../assets/spaceShip.gif cannot be found");
  std::shared_ptr<t_player>  ptr(new t_player({0, static_cast<float>(playerId * 32)}, {1, 1}));
  
  RessourceManager::instance().save(playerTexture);
  ptr->image = std::shared_ptr<AnimatedSprites>(new AnimatedSprites(sf::FloatRect(0, static_cast<float>(playerId * 32), 66, 32), 5,
                                                                    playerTexture->getSize().y, playerTexture));
  ptr->image->setCurrentAnim(static_cast<uint8_t>(Player::Animation::NORMAL));
  
  _players[playerId] = ptr;
}
