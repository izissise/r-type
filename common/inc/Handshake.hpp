#ifndef _HANDSHAKE_HPP_
# define _HANDSHAKE_HPP_

# include <string>
# include "APacket.hpp"

namespace Packet {
  class Handshake : public APacket
  {
  public:
    Handshake(const std::string &, uint16_t protocoleVersion);
    
    virtual const std::string to_bytes_body() const;
    virtual void from_bytes_body(const std::string &bytes);
    
    uint32_t getProtocolVersion() const;
    const std::string &getLogin() const;
    
  private:
    uint32_t      _protocolVersion;
    std::string   _login;
  };
}

#endif