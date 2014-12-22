#include "UDPConnectionProblem.hpp"
#include "Game.hpp"

template<>
std::map<Packet::APacket::PacketType, size_t (Game::*)(const Network::Buffer&)> RtypeProtoHelper<Game>::_netWorkBinds =
{
  { Packet::APacket::PacketType::SHORTRESPONSE, &Game::netShortResponse },
};

Game::Game(const sf::FloatRect &rect)
: Panel(rect), _network(Network::NetworkFactory::createNetwork()), _begin(false), _player({0, 0}, {1, 1}), _scrollSpeed(5)
{
  auto playerTexture = RessourceManager::instance().getTexture("../assets/spaceShip.gif");
  auto background = RessourceManager::instance().getTexture("../assets/gameBackground.png");
  std::shared_ptr<sf::Sprite> playerSprite(new sf::Sprite(*playerTexture));
  
  playerSprite->setTextureRect(sf::IntRect(66, 0, 33, 16));
  _image = std::shared_ptr<Image>(new Image(playerSprite, sf::FloatRect(0, 0, 66, 32)));
  _background = std::shared_ptr<Image>(new Image(std::shared_ptr<sf::Sprite>(new sf::Sprite(*background)),
                                                {0, 0, static_cast<float>(background->getSize().x), 900}));
}

void  Game::movePlayer(int axis, float speed)
{
  auto pos = _image->getPosition();
  
  if (axis == 1)
    pos.x += speed;
  else
    pos.y += speed;
  if (!(pos.x < 0 || pos.x + _image->getSize().x > 1600 || pos.y < 0 || pos.y + _image->getSize().y > 900))
    _image->setPosition(pos);
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
  _image->draw(win);
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
  float x = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
  float y =sf::Joystick::getAxisPosition(0, sf::Joystick::Y);
  
  if (isConnected())
    _network->poll();
//  Panel::update(event, timeElapsed);
  if (y != 0)
    movePlayer(0, _player.getSpeed().y * y * timeElapsed);
  if (x != 0)
    movePlayer(1, _player.getSpeed().x * x * timeElapsed);
  if ((event.type == sf::Event::JoystickButtonPressed && event.joystickButton.button == 11)
       || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space))
    std::cout << "Fire" << std::endl;
  if (event.type == sf::Event::KeyPressed)
  {
    switch (event.key.code)
    {
      case sf::Keyboard::Up :
        movePlayer(0, -_player.getSpeed().y * timeElapsed);
        break;
      case sf::Keyboard::Down :
        movePlayer(0, _player.getSpeed().y * timeElapsed);
        break;
      case sf::Keyboard::Left :
        movePlayer(1, -_player.getSpeed().x * timeElapsed);
        break;
      case sf::Keyboard::Right :
        movePlayer(1, _player.getSpeed().x * timeElapsed);
        break;
      default:
        break;
    }
  }
}

bool  Game::connect(const std::string &ip, const std::string &port, const std::string &login)
{
  try {
    std::shared_ptr<Network::ABasicSocket> socket = Network::NetworkFactory::createConnectSocket(ip, port, Network::ASocket::SockType::UDP);
    _network->registerClient(socket);
    setSocket(socket);
    _writeBuff.writeBuffer(Packet::Handshake(login).to_bytes());
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
  std::cout << "Call" << std::endl;
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
