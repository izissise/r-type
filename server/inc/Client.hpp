#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED

#include <memory>
#include <string>
#include <map>

#include "APacket.hpp"
#include "ClientHelper.hpp"
#include "ABasicSocket.hpp"
#include "RingBuffer.hpp"

class Server;

class Client : public Network::SocketClientHelper, public std::enable_shared_from_this<Client>
{
private:
  static std::map<Packet::APacket::PacketType, bool (Client::*)(const Network::Buffer&)> _netWorkBinds;

public:
  Client(const std::shared_ptr<Network::ABasicSocket>& sock, Server& serv);
  ~Client() = default;

protected:
  void onRead(size_t readSize) override;
  void onWrite(size_t writeSize) override;
  void onDisconnet() override;

private:
  bool netShortResponse(const Network::Buffer& data);
  bool netHandshake(const Network::Buffer& data);
  bool netGetListRoom(const Network::Buffer& data);
  bool netCreateRoom(const Network::Buffer& data);
  bool netJoinRoom(const Network::Buffer& data);

private:
  Server& _server;

  std::string _login;
  int		  _protoVersion;
};

#endif
