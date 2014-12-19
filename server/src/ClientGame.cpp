#include "ClientGame.hpp"

ClientGame::ClientGame(const std::shared_ptr<Network::Identity>& id,
                       const std::weak_ptr<Network::AListenSocket>& listener)
  : IdentityClientHelper(id, listener)
{
}

void ClientGame::onRead()
{

}
