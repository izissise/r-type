#include <iostream>
#include "Handshake.hpp"

namespace Packet {
  uint16_t Handshake::headerNumber = static_cast<uint16_t>(APacket::PacketType::HANDSHAKE);

  Handshake::Handshake()
  : APacket(PacketType::HANDSHAKE), _protocolVersion(0), _login("")
  {

  }

  Handshake::Handshake(const std::string &login)
  : APacket(PacketType::HANDSHAKE), _protocolVersion(PROTOCOLE_VERSION), _login(login)
  {

  }

  std::string Handshake::to_bytes_body() const
  {
    std::string ret = "";
    fill_bytes(ret, _protocolVersion);
    fill_bytes(ret, static_cast<uint16_t>(_login.length()));
    ret += _login;
    return (ret);
  }

  void Handshake::from_bytes_body(const std::string &bytes)
  {
    std::size_t pos = 1;
    uint16_t    loginLength = 0;

    get_bytes(bytes, pos, _protocolVersion);
    get_bytes(bytes, pos, loginLength);
    for (;pos < bytes.length();++pos)
      _login += bytes[pos];
    if (_login.length() != loginLength)
      throw (std::runtime_error("Parse Failed: the login size is false"));
  }

  uint32_t Handshake::getProtocolVersion() const
  {
    return _protocolVersion;
  }

  const std::string &Handshake::getLogin() const
  {
    return _login;
  }
}
