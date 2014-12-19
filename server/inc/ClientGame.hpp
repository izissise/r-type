#ifndef CLIENTGAME_H
#define CLIENTGAME_H

#include <string>
#include <map>
#include <memory>

#include "ClientHelper.hpp"
#include "APacket.hpp"

class ClientGame : public Network::IdentityClientHelper, public std::enable_shared_from_this<ClientGame>
{
public:
  ClientGame();
  virtual ~ClientGame() = default;
protected:
  void onRead() override;
};

#endif // CLIENTGAME_H
