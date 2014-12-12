#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED

#include <memory>

#include "ClientHelper.hpp"
#include "ABasicSocket.hpp"
#include "RingBuffer.hpp"

class Client : public Network::SocketClientHelper, public std::enable_shared_from_this<Client>
{
public:
  Client(const std::shared_ptr<Network::ABasicSocket>& sock);
  ~Client() = default;

protected:
  void onRead(size_t readSize) override;
  void onWrite(size_t writeSize) override;
};

#endif
