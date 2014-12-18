#include "StartGame.hpp"

namespace Packet {
StartGame::StartGame()
  : APacket(APacket::PacketType::STARTGAME), _ip(), _port()
{

}

StartGame::StartGame(const std::string& ip, uint16_t port)
  : APacket(APacket::PacketType::STARTGAME), _ip(ip), _port(port)
{

}

std::string StartGame::to_bytesNoHeader() const
{
  std::string ret = "";
  fill_bytes(ret, static_cast<uint16_t>(_ip.length()));
  ret += _ip;
  fill_bytes(ret, _port);
  return (ret);
}

size_t StartGame::from_bytes(const std::string &bytes)
{
  size_t pos = 0;
  uint16_t length;

  _ip = "";
  _port = 0;
  get_bytes(bytes, pos, length);
  for (; pos < bytes.length() && pos < length + sizeof(length); ++pos)
    _ip += bytes[pos];
  if (pos - sizeof(length) != length)
    throw APacket::PackerParsingError("The size of the ip is not right");
  get_bytes(bytes, pos, _port);
  return pos;
}
};
