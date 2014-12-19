#ifndef _STARTGAME_HPP_
# define _STARTGAME_HPP_

# include "APacket.hpp"

namespace Packet {
  class StartGame: public APacket
  {
  public:
    StartGame();
    StartGame(const std::string& ip, uint16_t port);

    const std::string &getIp() const {return _ip;};
    uint16_t getPort() const {return _port;};

    virtual std::string to_bytesNoHeader() const override;
    virtual size_t from_bytes(const std::string &bytes) override;

  private:
    std::string   _ip;
    uint16_t      _port;
  };
};

#endif