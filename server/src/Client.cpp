#include "Client.hpp"

Client::Client(const std::shared_ptr<Network::ABasicSocket>& sock)
  : _socket(sock)
{
}
