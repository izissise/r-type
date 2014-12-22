#ifndef _GETLISTPLAYER_HPP_
# define _GETLISTPLAYER_HPP_

# include <string>
# include <vector>
# include "APacket.hpp"

namespace Packet {
  typedef struct {
    std::string name;
    uint16_t    id;
  } PlayerClient;

  class GetListPlayer: public APacket
  {
  public:
    GetListPlayer();
    GetListPlayer(const std::vector<PlayerClient> &);

    const std::vector<PlayerClient>  &getPlayerList() const;

    std::string to_bytesNoHeader() const override;
    size_t from_bytes(const std::string &bytes) override;

  private:
    std::vector<PlayerClient>       _list;
  };
};

#endif