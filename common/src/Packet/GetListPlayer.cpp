#include "Packet/GetListPlayer.hpp"

namespace Packet {
  GetListPlayer::GetListPlayer()
  : APacket(PacketType::GETLISTPLAYER), _list()
  {

  }

  GetListPlayer::GetListPlayer(const std::vector<PlayerClient> &list)
  : APacket(PacketType::GETLISTPLAYER), _list(list)
  {

  }

  const std::vector<PlayerClient>  &GetListPlayer::getPlayerList() const
  {
    return (_list);
  }

  std::string GetListPlayer::to_bytesNoHeader() const
  {
    std::string ret("");

    fill_bytes(ret, static_cast<uint16_t>(_list.size()));
    for (auto& it : _list)
    {
      fill_bytes(ret, static_cast<uint16_t>(it.name.length()));
      ret += it.name;
      fill_bytes(ret, it.id);
    }
    return (ret);
  }

  size_t GetListPlayer::from_bytes(const std::string &bytes)
  {
    size_t pos = 0;
    uint16_t    listSize;

    get_bytes(bytes, pos, listSize);
    _list.clear();
    for (uint16_t i = 0; i < listSize && pos < bytes.length(); ++i)
    {
      PlayerClient      tmp;
      uint16_t          nameSize;
      size_t size = 0;

      get_bytes(bytes, pos, nameSize);
      for (; pos < bytes.length() && size < nameSize; ++pos, ++size)
        tmp.name += bytes[pos];
      if (size != nameSize)
        throw APacket::PackerParsingError("The size of the player list is not right");
	  get_bytes(bytes, pos, tmp.id);
      _list.push_back(tmp);
    }
    if (_list.size() != listSize)
      throw APacket::PackerParsingError("The size of the player is not correct");
    return pos;
  }
};