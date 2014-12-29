#include "ClientGame.hpp"

#include <initializer_list>
#include <type_traits>

#include "ServerGame.hpp"
#include "Packet/Handshake.hpp"
#include "Packet/ShortResponse.hpp"

std::map<Packet::APacket::PacketType, size_t (ClientGame::*)(const Network::Buffer&)> ClientGame::_netWorkBinds =
{
  {Packet::APacket::PacketType::SHORTRESPONSE, &ClientGame::netShortResponse},
  {Packet::APacket::PacketType::HANDSHAKE, &ClientGame::netHandshake},
  {Packet::APacket::PacketType::MOVE, &ClientGame::netMovement},
};

ClientGame::ClientGame(const std::shared_ptr<Network::Identity>& id,
                       const std::weak_ptr<Network::AListenSocket>& listener,
                       ServerGame& game)
  : IdentityClientHelper(id, listener), _game(game)
{
}

void ClientGame::sendPacket(const Packet::APacket& pack)
{
  _writeBuff.writeBuffer(pack);
  writeData();
}

void ClientGame::onRead()
{
  const size_t headerSize = sizeof(uint16_t);
  bool		   incomplete = false;
  Network::Buffer buff;
  Packet::APacket::PacketType pack;

  while (_readBuff.getLeftRead() && !incomplete)
    {
      _readBuff.readBuffer(buff, headerSize);
      pack = Packet::APacket::toPacketType(buff);
      if (pack != Packet::APacket::PacketType::UNKNOW)
        {
          buff.clear();
          _readBuff.readBuffer(buff, _readBuff.getLeftRead());
          try {
              size_t (ClientGame::*meth)(const Network::Buffer&) = _netWorkBinds.at(pack);
              try {
                  size_t nbUsed = (this->*meth)(buff);
                  _readBuff.rollbackReadBuffer(buff.size() - nbUsed);
                }
              catch (Packet::APacket::PackerParsingError& e)
                {
                  incomplete = true;
                }
            }
          catch (std::out_of_range& e)
            {
              _readBuff.rollbackReadBuffer(headerSize - 1);
            }
        }
      else
        {
          _readBuff.rollbackReadBuffer(headerSize - 1);
          std::cerr << "Received Unknown Packet" << std::endl;
        }
    }
}


void ClientGame::sendToOther(const Packet::APacket& pack)
{
  _game.broadcastPacketToOther(pack, shared_from_this());
}

/*
** Apacket binded functions
*/

size_t ClientGame::netShortResponse(const Network::Buffer& data)
{
  return Packet::ShortResponse().from_bytes(data);
}

size_t ClientGame::netHandshake(const Network::Buffer& data)
{
  Packet::Handshake hand;
  size_t  nbUsed;

  nbUsed = hand.from_bytes(data);

  _login = hand.getLogin();
  _protoVersion = hand.getProtocolVersion();
  _playerId = hand.getPlayerId();
  std::cout << _login << " " << _protoVersion << " " << _playerId << std::endl;
  Packet::ShortResponse rep(0);
  if (_protoVersion == PROTOCOLE_VERSION)
    rep = {1};
  _writeBuff.writeBuffer(rep);
  return nbUsed;
}

size_t ClientGame::netMovement(const Network::Buffer& data)
{
  Packet::MovePacket  movement;
  size_t              nbUsed;

  nbUsed = movement.from_bytes(data);
  sendToOther(movement);
  return nbUsed;
}

