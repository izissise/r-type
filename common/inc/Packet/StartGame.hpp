#ifndef _STARTGAME_HPP_
# define _STARTGAME_HPP_

# include "APacket.hpp"

namespace Packet {
  class StartGame: public APacket
  {
  public:
    StartGame();
    StartGame(uint16_t port, uint16_t pId);

    virtual ~StartGame() = default;

    uint16_t getPort() const {return _port;};
    uint16_t getPlayerId() const {return _playerId;};

    virtual std::string to_bytesNoHeader() const override;
    virtual size_t from_bytes(const std::string &bytes) override;

  private:
    uint16_t      _port;
    uint16_t	  _playerId;
  };
};

#endif