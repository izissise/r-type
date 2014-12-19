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

#include "ClientLobby.hpp"
#include "Lobby.hpp"
#include "ServerRoom.hpp"
#include "ServerGame.hpp"

class Server
{
public:
  Server(const std::vector<std::string>& args);
  ~Server() = default;

  void run();

  Lobby& getLobby() {return _lobby;};

  void broadcastAPacket(const Packet::APacket& pack) const;

  void unregisterClient(const std::shared_ptr<ClientLobby>& cli);

  void  broadcastRoomList() const;
  uint16_t createNewGame(const ServerRoom& gameInfo);

protected:
  void acceptNewClient(const std::weak_ptr<Network::AListenSocket>& that);

protected:
  Lobby 						          				_lobby;
  std::unique_ptr<Network::ANetwork>      				_net;
  std::deque<std::shared_ptr<Network::AListenSocket>>	_lobbyListener;
  std::deque<std::shared_ptr<ClientLobby>> 				_clients;
  ThreadPool							  				_threadPool;
  std::deque<std::shared_ptr<ServerGame>>				_games;
};

#endif
