#ifndef CLIENTGAME_H
#define CLIENTGAME_H

#include <string>
#include <map>
#include <memory>

#include "ClientHelper.hpp"
#include "Packet/APacket.hpp"
#include "Packet/MovePacket.hpp"

class ServerGame;

class ServerGame;

class ClientGame : public Network::IdentityClientHelper, public std::enable_shared_from_this<ClientGame>
{
private:
  static std::map<Packet::APacket::PacketType, size_t (ClientGame::*)(const Network::Buffer&)> _netWorkBinds;

public:
  ClientGame(const std::shared_ptr<Network::Identity>& id,
             const std::weak_ptr<Network::AListenSocket>& listener,
             ServerGame& game);
  virtual ~ClientGame() = default;

  void sendPacket(const Packet::APacket& pack);

  const std::string& getLogin() const {return _login;};
  size_t getId() const {return _playerId;};

protected:
  void onRead() override;
  void sendToOther(const Packet::APacket& pack);

private:
  size_t netShortResponse(const Network::Buffer& data);
  size_t netHandshake(const Network::Buffer& data);
  size_t netMovement(const Network::Buffer& data);
  size_t netMissile(const Network::Buffer& data);

private:
  ServerGame& 	_game;
  std::string   _login;
  int			_protoVersion;
  size_t		_playerId;
};

#endif // CLIENTGAME_H
