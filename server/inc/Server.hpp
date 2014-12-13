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

#include "Client.hpp"
#include "Lobby.hpp"

class Server
{
public:
  Server(const std::string& addr, const std::string& port);
  ~Server() = default;

  void run();

  Lobby& getLobby() {return _lobby;};

protected:
  void acceptNewClient(const std::weak_ptr<Network::AListenSocket>& that);

protected:
  Lobby 						          _lobby;
  std::unique_ptr<Network::ANetwork>      _net;
  std::shared_ptr<Network::AListenSocket> _lobbyListener;
  std::deque<std::shared_ptr<Client>>     _clients;
};

#endif
