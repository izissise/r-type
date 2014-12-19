#include "ClientLobby.hpp"

#include <initializer_list>
#include <type_traits>

#include "Server.hpp"

#include "Handshake.hpp"
#include "CreateRoom.hpp"
#include "AskListRoom.hpp"
#include "GetListRoom.hpp"
#include "ShortResponse.hpp"
#include "JoinRoom.hpp"
#include "Message.hpp"
#include "StartGame.hpp"

std::map<Packet::APacket::PacketType, size_t (ClientLobby::*)(const Network::Buffer&)> ClientLobby::_netWorkBinds =
{
  {Packet::APacket::PacketType::SHORTRESPONSE, &ClientLobby::netShortResponse},
  {Packet::APacket::PacketType::HANDSHAKE, &ClientLobby::netHandshake},
  {Packet::APacket::PacketType::ASKLISTROOM, &ClientLobby::netAskListRoom},
  {Packet::APacket::PacketType::CREATEROOM, &ClientLobby::netCreateRoom},
  {Packet::APacket::PacketType::JOINROOM, &ClientLobby::netJoinRoom},
  {Packet::APacket::PacketType::READYGAME, &ClientLobby::netReadyGame},
  {Packet::APacket::PacketType::LEAVEROOM, &ClientLobby::netLeaveRoom},
  {Packet::APacket::PacketType::MESSAGE, &ClientLobby::netMessage}
};

ClientLobby::ClientLobby(const std::shared_ptr<Network::ABasicSocket>& sock, Server& serv)
  : SocketClientHelper(sock), _server(serv), _currentRoom(-1), _isGameReady(false)
{
}

void ClientLobby::onRead(size_t nbRead)
{
  const size_t headerSize = sizeof(uint16_t);
  Network::Buffer buff;
  Packet::APacket::PacketType pack;

  if (nbRead == 0)
    return;
  while (_readBuff.getLeftRead() >= headerSize)
    {
      _readBuff.readBuffer(buff, headerSize);
      pack = Packet::APacket::toPacketType(buff);
      if (pack != Packet::APacket::PacketType::UNKNOW)
        {
//      std::cout << std::hex << "0x" << static_cast<int>(buff[0]) << " " << std::dec;
//      std::cout << std::hex << "0x" << static_cast<int>(buff[1]) << " " << std::dec;
          buff.clear();
          _readBuff.readBuffer(buff, _readBuff.getLeftRead());
          try {
              size_t (ClientLobby::*meth)(const Network::Buffer&) = _netWorkBinds.at(pack);
              try {
//                  for (auto& i : buff)
//                    {
//                      std::cout << std::hex << "0x" << static_cast<int>(i) << " " << std::dec;
//                    }
//                  std::cout << std::endl;
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

void ClientLobby::onWrite(size_t)
{
}

void ClientLobby::onDisconnet()
{
  std::shared_ptr<ClientLobby> tmp = shared_from_this();

  std::cout << "Unregistered client" << std::endl;
  _server.unregisterClient(tmp);
  if (_currentRoom != -1)
    _server.getLobby().leaveRoom(tmp, _currentRoom);
}


void ClientLobby::sendPacket(const Packet::APacket& pack)
{
  _writeBuff.writeBuffer(pack);
}

void ClientLobby::startGame(uint16_t port)
{
  sendPacket(Packet::StartGame(_socket->getIpAddr(), port));
}


/*
** Apacket binded functions
*/

size_t ClientLobby::netShortResponse(const Network::Buffer& data)
{
  return Packet::ShortResponse().from_bytes(data);
}

size_t ClientLobby::netHandshake(const Network::Buffer& data)
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

size_t ClientLobby::netAskListRoom(const Network::Buffer& data)
{
  Packet::AskListRoom ask;
  size_t  nbUsed;

  nbUsed = ask.from_bytes(data);
  std::cout << "Ask for Room" << std::endl;
  _server.broadcastRoomList();
  return nbUsed;
}

size_t ClientLobby::netCreateRoom(const Network::Buffer& data)
{
  Packet::CreateRoom cr;
  size_t  nbUsed;

  nbUsed = cr.from_bytes(data);
  t_room room = *(cr.getRoom());
  size_t rId = _server.getLobby().newRoom(room);
  std::cout << "New room: " << room.name << std::endl;
  bool joined = _server.getLobby().joinRoom(shared_from_this(), rId);
  _writeBuff.writeBuffer(Packet::ShortResponse(joined));

  if (joined)
    {
      if (_currentRoom != -1)
        _server.getLobby().leaveRoom(shared_from_this(), rId);
      _currentRoom = rId;
      _server.getLobby().getRoom(_currentRoom).sendPlayerList();
      _server.broadcastRoomList();
      _currentRoom = rId;
    }
  return nbUsed;
}

size_t ClientLobby::netJoinRoom(const Network::Buffer& data)
{
  Packet::JoinRoom jr;
  size_t  nbUsed;

  nbUsed = jr.from_bytes(data);
  size_t rId = jr.getRoomId();
  bool joined = _server.getLobby().joinRoom(shared_from_this(), rId);
  _writeBuff.writeBuffer(Packet::ShortResponse(joined));
  if (joined)
    {
      if (_currentRoom != -1)
        _server.getLobby().leaveRoom(shared_from_this(), _currentRoom);
      _currentRoom = rId;
      _server.getLobby().getRoom(_currentRoom).sendPlayerList();
      _server.broadcastRoomList();
    }
  return nbUsed;
}

size_t ClientLobby::netReadyGame(const Network::Buffer&)
{
  if (_currentRoom != -1)
    {
      _server.getLobby().getRoom(_currentRoom).broadcastAPacket(Packet::Message(_login + " is ready."));
      _isGameReady = true;
      _server.getLobby().getRoom(_currentRoom).tryLaunchGame(_server);
    }
  return 0;
}

size_t ClientLobby::netLeaveRoom(const Network::Buffer&)
{
  _server.getLobby().leaveRoom(shared_from_this(), _currentRoom);
  _currentRoom = -1;
  _isGameReady = false;
  _server.broadcastRoomList();
  return 0;
}

size_t ClientLobby::netMessage(const Network::Buffer& data)
{
  Packet::Message msg;
  size_t  nbUsed;

  nbUsed = msg.from_bytes(data);
  if (_currentRoom != -1)
    {
      try {
          _server.getLobby().getRoom(_currentRoom).broadcastAPacket(msg);
        }
      catch (std::out_of_range& e)
        {
          std::cerr << "No such room!" << std::endl;
        }
    }
  return nbUsed;
}
