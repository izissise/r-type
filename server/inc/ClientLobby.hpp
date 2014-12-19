#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED

#include <memory>
#include <string>
#include <map>

#define PROTOCOLEVERSION 0x01000000

#include "APacket.hpp"
#include "ClientHelper.hpp"
#include "ABasicSocket.hpp"
#include "RingBuffer.hpp"

class Server;

class ClientLobby : public Network::SocketClientHelper, public std::enable_shared_from_this<ClientLobby>
{
private:
  static std::map<Packet::APacket::PacketType, size_t (ClientLobby::*)(const Network::Buffer&)> _netWorkBinds;

public:
  ClientLobby(const std::shared_ptr<Network::ABasicSocket>& sock, Server& serv);
  ~ClientLobby() = default;

  const std::string& getLogin() const {return _login;};

  void sendPacket(const Packet::APacket& pack);
  void startGame(uint16_t port);

  bool isReadyForGame() const {return _isGameReady;};

protected:
  void onRead(size_t readSize) override;
  void onWrite(size_t writeSize) override;
  void onDisconnet() override;

private:
  size_t netShortResponse(const Network::Buffer& data);
  size_t netHandshake(const Network::Buffer& data);
  size_t netAskListRoom(const Network::Buffer& data);
  size_t netCreateRoom(const Network::Buffer& data);
  size_t netJoinRoom(const Network::Buffer& data);
  size_t netReadyGame(const Network::Buffer& data);
  size_t netLeaveRoom(const Network::Buffer& data);
  size_t netMessage(const Network::Buffer& data);

private:
  Server& _server;

  std::string _login;
  int		  _protoVersion;
  int		  _currentRoom;
  bool		  _isGameReady;
};

#endif
