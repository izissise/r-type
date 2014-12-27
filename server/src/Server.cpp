#include "Server.hpp"

#include <sstream>

#include "Packet/GetListRoom.hpp"

Server::Server(const std::vector<std::string>& args)
  : _net(Network::NetworkFactory::createNetwork()),
    _threadPool(5),
    _dlMonsterManager("createMonster")
{
  decltype(_udpPortRange) tmpPortRange = std::make_tuple(0, 0);
  std::swap(_udpPortRange, tmpPortRange);
  auto addListen = [this](const std::string & addr, const std::string & port) {
    std::shared_ptr<Network::AListenSocket> lobbyListener = Network::NetworkFactory::createListenSocket(addr, port, Network::ASocket::SockType::TCP, true);
    lobbyListener->setAcceptorCallback(std::bind(&Server::acceptNewClient, this, std::placeholders::_1));
    _net->registerListener(lobbyListener);
    _lobbyListener.push_back(lobbyListener);
    std::cout << "Server listening on " << lobbyListener->getListeningIpAddr() << ":"
              << lobbyListener->getListeningPort() << std::endl;
  };

  bool oneListener = false;
  for (size_t i = 1; i < args.size(); ++i)
    {
      try
        {
          if ((i + 3 < args.size()) && args[i] == "-l" && args[i + 2] == "-p")
            {
              addListen(args[i + 1], args[i + 3]);
              oneListener = true;
              i += 2;
            }
          else if ((i + 1 < args.size()) && args[i] == "-p")
            {
              addListen("0.0.0.0", args[i + 1]);
              oneListener = true;
              addListen("::1", args[i + 1]);
              i += 1;
            }
          else if ((i + 1 < args.size()) && args[i] == "-l")
            {
              addListen(args[i + 1], "8000");
              oneListener = true;
              i += 1;
            }
        }
      catch (std::exception& e) {
          std::cerr << "Can't bind address: " << e.what() << std::endl;
        }
      if (args[i] == "-h" || args[i] == "--help")
        {
          std::cout << "Usage: " << args[0] << " [[-l ip] | [-p port]]" << std::endl;
        }
      else if (args[i] == "-r" && (i + 1 < args.size()))
        {
          try
            {
              setUdpPortRange(args[i + 1]);
            }
          catch (std::exception& e) {
              std::cerr << e.what() << std::endl;
            }
          i += 1;

        }
    }
  if (!oneListener)
    {
      addListen("0.0.0.0", "8000");
      addListen("::1", "8000");
    }

  try
    {
      _dlMonsterManager.loadAllInDirectory("monsters");
    }
  catch (std::exception& e) {
      std::cerr << e.what() << std::endl;
    }
}

void Server::run()
{
  while (true)
    _net->poll(true);
}

void Server::broadcastAPacket(const Packet::APacket& pack) const
{
  for (auto& i : _clients)
    {
      i->sendPacket(pack);
    }
}

void Server::broadcastRoomList() const
{
  Packet::GetListRoom glr(_lobby.roomLists());
  broadcastAPacket(glr);
}

void Server::unregisterClient(const std::shared_ptr<ClientLobby>& cli)
{
  _clients.erase(std::remove_if(_clients.begin(), _clients.end(),
  [&cli](std::shared_ptr<ClientLobby>& cl) -> bool {
    return (cli == cl);
  }), _clients.end());
}

void Server::acceptNewClient(const std::weak_ptr<Network::AListenSocket>& that)
{
  auto listener = that.lock();
  std::shared_ptr<Network::ABasicSocket> nClientSock = listener->acceptClient();
  auto nclient = std::make_shared<ClientLobby>(nClientSock, *this);

  _clients.push_back(nclient);
  _net->registerClient(nClientSock);
  std::cout << "New client: " << nClientSock->getRemoteIpAddr() << ":" << nClientSock->getRemotePort() << std::endl;
}

uint16_t Server::createNewGame(const ServerRoom& gameInfo)
{
  auto toStr = [](uint16_t p) {std::stringstream ss(""); ss << p; return ss.str();};
  uint16_t minPort = std::get<0>(_udpPortRange);
  uint16_t maxPort = std::get<1>(_udpPortRange);

  std::string strPort = (minPort == 0) ? "" : toStr(minPort);
  auto game = std::make_shared<ServerGame>(gameInfo, strPort, _dlMonsterManager);
  minPort++;
  if (minPort > maxPort)
    setUdpPortRange(std::make_tuple(0, 0));
  else
    setUdpPortRange(std::make_tuple(minPort, maxPort));

  _games.push_back(game);
  _threadPool.addTask(&ServerGame::run, game);
  return [&game]() {std::stringstream ss(""); ss << game->listeningPort(); uint16_t port; ss >> port; return port;}();
}

void Server::setUdpPortRange(const std::string& portRange)
{
  uint16_t minPort = 0;
  uint16_t maxPort = 0;
  std::string tmppr = [](std::string context, const std::string & from, const std::string & to) {
    size_t lookHere = 0;
    size_t foundHere;
    while((foundHere = context.find(from, lookHere)) != std::string::npos)
      {
        context.replace(foundHere, from.size(), to);
        lookHere = foundHere + to.size();
      }
    return context;
  }(portRange, ":", " ");
  std::stringstream tmp(tmppr);
  if (!(tmp >> minPort >> maxPort))
    {
      throw std::runtime_error("Wrong portRange format.");
    }

  decltype(_udpPortRange) tmpPortRange = std::make_tuple(minPort, maxPort);
  std::swap(_udpPortRange, tmpPortRange);
}
