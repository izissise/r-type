#include "GetListRoom.hpp"

namespace Packet {
  uint16_t GetListRoom::headerNumber = 2;

  GetListRoom::GetListRoom()
  : APacket(PacketType::GETLISTROOM), _list()
  {

  }

  GetListRoom::GetListRoom(const std::vector<t_room> &list)
  : APacket(PacketType::GETLISTROOM), _list(list)
  {

  }

  const std::vector<t_room>  &GetListRoom::getListRoom() const
  {
    return (_list);
  }

  std::string GetListRoom::to_bytes_body() const
  {
    std::string ret("");

    fill_bytes(ret, static_cast<uint32_t>(_list.size()));
    for (auto it : _list)
    {
      fill_bytes(ret, static_cast<uint32_t>(it.name.length()));
      ret += it.name;
      fill_bytes(ret, it.id);
      fill_bytes(ret, it.playerMax);
      fill_bytes(ret, it.nbPlayer);
    }
    return (ret);
  }

  void GetListRoom::from_bytes_body(const std::string &bytes)
  {
    std::size_t pos = 1;
    uint32_t    listSize;

    get_bytes(bytes, pos, listSize);
    _list.clear();
    for (uint32_t i = 0;i < listSize && pos < bytes.length();++i)
    {
      t_room      tmp;
      uint32_t    nameSize;
      std::size_t size = 0;

      get_bytes(bytes, pos, nameSize);
      for (; pos < bytes.length() && size < nameSize; ++pos, ++size)
        tmp.name += bytes[pos];
      if (size != nameSize)
        throw std::runtime_error("The size of the room's name is not right");
      get_bytes(bytes, pos, tmp.id);
      get_bytes(bytes, pos, tmp.playerMax);
      get_bytes(bytes, pos, tmp.nbPlayer);

      _list.push_back(tmp);
    }
    if (_list.size() != listSize)
      throw std::runtime_error("The size of the room's list is not correct");
  }

}