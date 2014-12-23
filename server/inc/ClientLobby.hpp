#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED

#include <memory>
#include <string>
#include <map>

#define PROTOCOLEVERSION 0x01000000

#include "Packet/APacket.hpp"
#include "ABasicSocket.hpp"
#include "RingBuffer.hpp"
#include "RtypeProtoHelper.hpp"

class Server;

class ClientLobby : public RtypeProtoHelper<ClientLobby>, public std::enable_shared_from_this<ClientLobby>
{
 friend RtypeProtoHelper<ClientLobby>;

public:
  ClientLobby(const std::shared_ptr<Network::ABasicSocket>& sock, Server& serv);
  ~ClientLobby() = default;

  const std::string& getLogin() const {return _login;};

  void sendPacket(const Packet::APacket& pack);
  void startGame(uint16_t port, size_t id);

  bool isReadyForGame() const {return _isGameReady;};

  const std::string& getConnectionAddress() const {return _socket->getIpAddr();};

protected:
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
