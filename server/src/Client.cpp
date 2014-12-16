#include "Client.hpp"

#include <initializer_list>
#include <type_traits>

#include "Server.hpp"

#include "Handshake.hpp"
#include "CreateRoom.hpp"
#include "AskListRoom.hpp"
#include "GetListRoom.hpp"
#include "ShortResponse.hpp"

std::map<Packet::APacket::PacketType, size_t (Client::*)(const Network::Buffer&)> Client::_netWorkBinds =
{
  {Packet::APacket::PacketType::SHORTRESPONSE, &Client::netShortResponse},
  {Packet::APacket::PacketType::HANDSHAKE, &Client::netHandshake},
  {Packet::APacket::PacketType::ASKLISTROOM, &Client::netAskListRoom},
  {Packet::APacket::PacketType::CREATEROOM, &Client::netCreateRoom},
  {Packet::APacket::PacketType::JOINROOM, &Client::netJoinRoom}
};

Client::Client(const std::shared_ptr<Network::ABasicSocket>& sock, Server& serv)
  : SocketClientHelper(sock), _server(serv)
{
}

void Client::onRead(size_t nbRead)
{
  const size_t headerSize = sizeof(uint16_t);
  Network::Buffer buff;
  Packet::APacket::PacketType pack;
  bool isPacket = false;

  if (nbRead == 0)
    return;
  while (!isPacket && _readBuff.getLeftRead() >= headerSize)
    {
      _readBuff.readBuffer(buff, headerSize);
      pack = Packet::APacket::toPacketType(buff);
      if (pack != Packet::APacket::PacketType::UNKNOW)
        {
          buff.clear();
          _readBuff.readBuffer(buff, _readBuff.getLeftRead());
          isPacket = true;
          try {
              size_t (Client::*meth)(const Network::Buffer&) = _netWorkBinds.at(pack);
              try {
                  size_t nbUsed = (this->*meth)(buff);
                  _readBuff.rollbackReadBuffer(buff.size() - nbUsed);
                }
              catch (std::exception& e)
                {
                  _readBuff.rollbackReadBuffer(buff.size() - 1);
                }
            }
          catch (std::exception& e)
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

void Client::onWrite(size_t)
{
}

void Client::onDisconnet()
{
  std::shared_ptr<Client> tmp = shared_from_this();

  std::cout << "Unregistered client" << std::endl;
  _server.unregisterClient(tmp);
}


void Client::sendPacket(const Packet::APacket& pack)
{
  _writeBuff.writeBuffer(pack.to_bytes());
}


/*
** Apacket binded functions
*/

size_t Client::netShortResponse(const Network::Buffer&)
{
  return 0;
}

size_t Client::netHandshake(const Network::Buffer& data)
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
  _writeBuff.writeBuffer(rep.to_bytes());
  return nbUsed;
}

size_t Client::netAskListRoom(const Network::Buffer& data)
{
  Packet::AskListRoom ask;
  size_t  nbUsed;

  nbUsed = ask.from_bytes(data);
  std::cout << "Ask for Room" << std::endl;
  Packet::GetListRoom room(_server.getLobby().roomLists());
  _writeBuff.writeBuffer(room.to_bytes());
  return nbUsed;
}

size_t Client::netCreateRoom(const Network::Buffer& data)
{
  Packet::CreateRoom cr;
  size_t  nbUsed;

  nbUsed = cr.from_bytes(data);
  t_room room = *(cr.getRoom());
  size_t rId = _server.getLobby().newRoom(room);
  std::cout << "New room: " << room.name << std::endl;
  bool joined = _server.getLobby().joinRoom(shared_from_this(), rId);
  Packet::ShortResponse rep(0);
  if (joined)
    rep = {1};
  _writeBuff.writeBuffer(rep.to_bytes());

  Packet::GetListRoom glr(_server.getLobby().roomLists());
  _server.broadcastAPacket(glr);
  return nbUsed;
}

size_t Client::netJoinRoom(const Network::Buffer&)
{
  return 0;
}
