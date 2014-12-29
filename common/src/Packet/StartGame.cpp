#include "Packet/StartGame.hpp"

namespace Packet {
StartGame::StartGame()
  : APacket(APacket::PacketType::STARTGAME), _port()
{

}

StartGame::StartGame(uint16_t port, uint16_t pId)
  : APacket(APacket::PacketType::STARTGAME), _port(port), _playerId(pId)
{

}

std::string StartGame::to_bytesNoHeader() const
{
  std::string ret = "";
  fill_bytes(ret, _port);
  fill_bytes(ret, _playerId);
  return (ret);
}

size_t StartGame::from_bytes(const std::string &bytes)
{
  size_t pos = 0;

  _port = 0;
  get_bytes(bytes, pos, _port);
  get_bytes(bytes, pos, _playerId);
  return pos;
}
};
