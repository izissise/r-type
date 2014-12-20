#include "ClientGame.hpp"

#include <initializer_list>
#include <type_traits>

#include "Handshake.hpp"
#include "ShortResponse.hpp"

std::map<Packet::APacket::PacketType, size_t (ClientGame::*)(const Network::Buffer&)> ClientGame::_netWorkBinds =
{
  {Packet::APacket::PacketType::SHORTRESPONSE, &ClientGame::netShortResponse},
  {Packet::APacket::PacketType::HANDSHAKE, &ClientGame::netHandshake}
};

ClientGame::ClientGame(const std::shared_ptr<Network::Identity>& id,
                       const std::weak_ptr<Network::AListenSocket>& listener)
  : IdentityClientHelper(id, listener)
{

}

void ClientGame::onRead()
{
  const size_t headerSize = sizeof(uint16_t);
  Network::Buffer buff;
  Packet::APacket::PacketType pack;

  while (_readBuff.getLeftRead() >= headerSize)
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
                  _readBuff.rollbackReadBuffer(buff.size() - 1);
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
  std::cout << _login << " " << _protoVersion << std::endl;
  Packet::ShortResponse rep(0);
  if (_protoVersion == PROTOCOLE_VERSION)
    rep = {1};
  _writeBuff.writeBuffer(rep);
  return nbUsed;
}
