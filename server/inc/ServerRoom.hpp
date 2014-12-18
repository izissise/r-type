#ifndef SERVERROOM_H
#define SERVERROOM_H

#include <vector>
#include <string>
#include <memory>

#include "Client.hpp"
#include "APacket.hpp"

class ServerRoom
{
public:
  ServerRoom(const std::string& name, size_t id, size_t nbPMax);
  virtual ~ServerRoom() = default;

  size_t getId() const {return _id;};
  const std::string& getName() const {return _name;};
  size_t getNbPlayers() const {return _clients.size();};
  size_t getMaxNbPlayer() const {return _nbPMax;};

  const std::vector<std::shared_ptr<Client>>& getPlayerList() const {return _clients;};

  void broadcastAPacket(const Packet::APacket& pack) const;

  bool addPlayer(const std::shared_ptr<Client>& p);
  void removePlayer(const std::shared_ptr<Client>& p);
  void sendPlayerList() const;
  void tryLaunchGame(Server& server) const;

protected:
  std::vector<std::shared_ptr<Client>>   _clients;
  size_t								 _id;
  std::string 						     _name;
  size_t								 _nbPMax;
};

#endif // SERVERROOM_H
