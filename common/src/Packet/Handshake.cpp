#include <iostream>
#include "Packet/Handshake.hpp"

namespace Packet {
  Handshake::Handshake()
  : APacket(PacketType::HANDSHAKE), _protocolVersion(0), _login("")
  {

  }

  Handshake::Handshake(const std::string &login, uint16_t id)
  : APacket(PacketType::HANDSHAKE), _protocolVersion(PROTOCOLE_VERSION),
    _login(login), _playerId(id)
  {

  }

  std::string Handshake::to_bytesNoHeader() const
  {
    std::string ret = "";
    fill_bytes(ret, _protocolVersion);
    fill_bytes(ret, static_cast<uint16_t>(_login.length()));
    ret += _login;
    fill_bytes(ret, _playerId);
    return (ret);
  }

  size_t Handshake::from_bytes(const std::string &bytes)
  {
    size_t pos = 0;
    uint16_t    loginLength = 0;

    get_bytes(bytes, pos, _protocolVersion);
    get_bytes(bytes, pos, loginLength);
    for (; pos < bytes.length() && _login.size() < loginLength; ++pos)
      _login += bytes[pos];
    if (_login.length() != loginLength)
      throw (APacket::PackerParsingError("Parse Failed: the login size is wrong"));
	get_bytes(bytes, pos, _playerId);
    return pos;
  }

  uint32_t Handshake::getProtocolVersion() const
  {
    return _protocolVersion;
  }

  const std::string &Handshake::getLogin() const
  {
    return _login;
  }
};
