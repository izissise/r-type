#include "UDPConnectionProblem.hpp"
#include "Game.hpp"

template<>
std::map<Packet::APacket::PacketType, size_t (Game::*)(const Network::Buffer&)> RtypeProtoHelper<Game>::_netWorkBinds =
{
  {Packet::APacket::PacketType::SHORTRESPONSE, &Game::netShortResponse },
};

Game::Game(const sf::FloatRect &rect)
: Panel(rect), _network(Network::NetworkFactory::createNetwork()), _begin(false)
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
    std::cout << "Connection Accepted" << std::endl;
    _begin = true;
  }
  else
    throw UDPConnectionProblem("Rejected By the server");
  return nbUsed;
}
