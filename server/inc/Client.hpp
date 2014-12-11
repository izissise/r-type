#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED

#include <memory>

#include "ClientHelper.hpp"
#include "ABasicSocket.hpp"
#include "RingBuffer.hpp"

class Client : public Network::ClientHelper, public std::enable_shared_from_this<Client>
{
public:
  Client(const std::shared_ptr<Network::ABasicSocket>& sock);
  ~Client() = default;

  void onReadeable() override;
  void onWritable() override;
protected:
};

#endif
