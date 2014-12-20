#ifndef CLIENTGAME_H
#define CLIENTGAME_H

#include <string>
#include <map>
#include <memory>

#include "ClientHelper.hpp"
#include "APacket.hpp"

class ClientGame : public Network::IdentityClientHelper, public std::enable_shared_from_this<ClientGame>
{
private:
  static std::map<Packet::APacket::PacketType, size_t (ClientGame::*)(const Network::Buffer&)> _netWorkBinds;

public:
  ClientGame(const std::shared_ptr<Network::Identity>& id,
             const std::weak_ptr<Network::AListenSocket>& listener);
  virtual ~ClientGame() = default;
protected:
  void onRead() override;

private:
  size_t netShortResponse(const Network::Buffer& data);
  size_t netHandshake(const Network::Buffer& data);

private:
  std::string   _login;
  int			_protoVersion;
};

#endif // CLIENTGAME_H
