#include "Game.hpp"

Game::Game(const sf::FloatRect &rect)
: Panel(rect), SocketClientHelper(), _network(Network::NetworkFactory::createNetwork())
{
  
}

Game::~Game()
{
  
}

void  Game::update(const sf::Event &event)
{
  if (isConnected())
    _network->poll();
  Panel::update(event);
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

void  Game::onRead(size_t sizeRead)
{
  std::cout << "Read" << std::endl;
}

void  Game::onWrite(size_t sizeWrite)
{
  std::cout << "Write" << std::endl;
}

void  Game::onDisconnet()
{
  std::cout << "UDP Disconnected" << std::endl;
}
