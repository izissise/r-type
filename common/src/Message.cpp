#include "Message.hpp"

namespace Packet {
  Message::Message()
  : APacket(APacket::PacketType::MESSAGE), _msg()
  {

  }

  Message::Message(const std::string &msg)
  : APacket(APacket::PacketType::MESSAGE), _msg(msg)
  {

  }

  const std::string &Message::getMsg() const
  {
    return (_msg);
  }

  std::string Message::to_bytesNoHeader() const
  {
    std::string ret = "";
    fill_bytes(ret, static_cast<uint16_t>(_msg.length()));
    ret += _msg;
    return (ret);
  }

  std::size_t Message::from_bytes(const std::string &bytes)
  {
    std::size_t pos = 0;
    uint16_t length;

    _msg = "";
    get_bytes(bytes, pos, length);
    for (; pos < bytes.length() && pos < length + sizeof(length); ++pos)
      _msg += bytes[pos];
    if (pos - sizeof(length) != length)
      throw APacket::PackerParsingError("The size of the room's name is not right");
    return pos;
  }
};