#ifndef SERVER_H_INCLUDED_
#define SERVER_H_INCLUDED_

#include <memory>
#include <functional>
#include <string>
#include <iostream>
#include <deque>

#include "NetworkFactory.hpp"
#include "ANetwork.hpp"
#include "AListenSocket.hpp"
#include "ABasicSocket.hpp"
#include "APacket.hpp"

#include "ThreadPool.hpp"

#include "Client.hpp"
#include "Lobby.hpp"
#include "ServerRoom.hpp"
#include "ServerGame.hpp"

class Server
{
public:
  Server(const std::string& addr, const std::string& port);
  ~Server() = default;

  void run();

  Lobby& getLobby() {return _lobby;};

  void broadcastAPacket(const Packet::APacket& pack) const;

  void unregisterClient(const std::shared_ptr<Client>& cli);

  void  broadcastRoomList() const;
  const std::shared_ptr<Network::AListenSocket>& createNewGame(const ServerRoom& gameInfo);

protected:
  void acceptNewClient(const std::weak_ptr<Network::AListenSocket>& that);

protected:
  Lobby 						          	_lobby;
  std::unique_ptr<Network::ANetwork>      	_net;
  std::shared_ptr<Network::AListenSocket>	_lobbyListener;
  std::deque<std::shared_ptr<Client>>    	_clients;
  ThreadPool							  	_threadPool;
  std::deque<std::shared_ptr<ServerGame>>	_games;
};

#endif
