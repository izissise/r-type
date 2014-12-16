#ifndef _HANDSHAKE_HPP_
# define _HANDSHAKE_HPP_

# include <iostream>
# include <string>
# include "APacket.hpp"

# define PROTOCOLE_VERSION 0x01000000

namespace Packet {
  class Handshake : public APacket
  {
  public:
    Handshake();
    Handshake(const std::string &);

    uint32_t getProtocolVersion() const;
    const std::string &getLogin() const;

    std::string to_bytes() const override;
    size_t from_bytes(const std::string &bytes) override;

  private:
    uint32_t      _protocolVersion;
    std::string   _login;
  };
};

#endif
