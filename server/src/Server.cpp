#include "Server.hpp"

#include <sstream>

#include "Packet/GetListRoom.hpp"

Server::Server(const std::vector<std::string>& args)
  : _net(Network::NetworkFactory::createNetwork()),
    _threadPool(5)
{
  auto addListen = [this](const std::string & addr, const std::string & port) {
    std::shared_ptr<Network::AListenSocket> lobbyListener = Network::NetworkFactory::createListenSocket(addr, port, Network::ASocket::SockType::TCP, true);
    lobbyListener->setAcceptorCallback(std::bind(&Server::acceptNewClient, this, std::placeholders::_1));
    _net->registerListener(lobbyListener);
    _lobbyListener.push_back(lobbyListener);
    std::cout << "Server listening on " << lobbyListener->getListeningIpAddr() << ":"
              << lobbyListener->getListeningPort() << std::endl;
  };


  size_t i;
  for (i = 1; i < args.size(); ++i)
    {
      try
        {
          if ((i + 3 < args.size()) && args[i] == "-h" && args[i + 2] == "-p")
            {
              addListen(args[i + 1], args[i + 3]);
              i += 2;
            }
          else if ((i + 1 < args.size()) && args[i] == "-p")
            {
              addListen("0.0.0.0", args[i + 1]);
              addListen("::1", args[i + 1]);
              i += 1;
            }
          else if ((i + 1 < args.size()) && args[i] == "-h")
            {
              addListen(args[i + 1], "8000");
              i += 1;
            }
        }
      catch (std::exception& e) {
          std::cerr << "Can't bind address: " << e.what() << std::endl;
        }
    }
  if (i == 1)
    {
      addListen("0.0.0.0", "8000");
      addListen("::1", "8000");
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
  std::shared_ptr<Network::AListenSocket> listener = that.lock();
  std::shared_ptr<Network::ABasicSocket> nClientSock = listener->acceptClient();
  std::shared_ptr<ClientLobby> nclient(new ClientLobby(nClientSock, *this));

  _clients.push_back(nclient);
  _net->registerClient(nClientSock);
  std::cout << "New client: " << nClientSock->getRemoteIpAddr() << ":" << nClientSock->getRemotePort() << std::endl;
}

uint16_t Server::createNewGame(const ServerRoom& gameInfo)
{
  std::shared_ptr<ServerGame> game(new ServerGame(gameInfo));

  _games.push_back(game);
  _threadPool.addTask(&ServerGame::run, game);
  return [&game]() {std::stringstream ss(""); ss << game->listeningPort(); uint16_t port; ss >> port; return port;}();
}
