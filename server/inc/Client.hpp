#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED

#include <memory>

#include "ABasicSocket.hpp"

class Client : public std::enable_shared_from_this<Client>
{
public:
  Client(const std::shared_ptr<Network::ABasicSocket>& sock);
  ~Client() = default;

  void onReadeable();
  void onWritable();
protected:
  std::shared_ptr<Network::ABasicSocket> _socket;
};

#endif
