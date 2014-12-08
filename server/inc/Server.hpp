#ifndef SERVER_H_INCLUDED_
#define SERVER_H_INCLUDED_

#include <memory>
#include <string>
#include <iostream>

#include "NetworkFactory.hpp"
#include "ANetwork.hpp"

class Server
{
public:
  Server(const std::string& addr, const std::string& port);
  ~Server();

protected:
	std::unique_ptr<Network::ANetwork> _net;
	std::shared_ptr<Network::AListenSocket> _listener;
};

#endif
