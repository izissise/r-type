#ifndef _APACKET_HPP_
# define _APACKET_HPP_

# include <iostream>
# include <string>
# include <stdexcept>
# include <cstdint>

# include "EnumChecker.hpp"

namespace Packet {

  class APacket
  {
  public:
    enum class PacketType
    {
      SHORTRESPONSE = 0, //0
      HANDSHAKE = 1, 	//1
      GETLISTROOM = 2, //2
      CREATEROOM = 3, //3
      JOINROOM = 4,	 //4
      UNKNOW,
    };

  public:
    APacket(Packet::APacket::PacketType type);
    virtual ~APacket() = default;

    operator std::string();

    std::string to_bytes() const;
    void from_bytes(const std::string &bytes);

    static Packet::APacket::PacketType toPacketType(uint16_t p);
	static Packet::APacket::PacketType toPacketType(const std::string& buff);

	static uint16_t fromPacketType(Packet::APacket::PacketType p) {return static_cast<uint16_t>(p);};

  protected:
    virtual std::string to_bytes_body() const = 0;
    virtual void from_bytes_body(const std::string &bytes) = 0;
    virtual uint16_t getHeaderNumber() const = 0;

    template <typename T>
    void fill_bytes(std::string &bytes, T nb) const
    {
      auto it = bytes.end();

      for (size_t i = 0; i < sizeof(T); ++i)
      {
        it = bytes.insert(it, (nb & 0xFF));
        nb = nb >> 8;
      }
    }

    template <typename T>
    void get_bytes(const std::string &bytes, size_t &pos, T &nb) const
    {
      size_t i;

      for (i = 0; i < sizeof(T) && pos + i != bytes.size(); ++i)
        nb = ((nb << 8) | bytes[pos + i]);
      pos += i;
      if (pos < sizeof(T) + 1)
        throw std::invalid_argument("Error while parsing packet");
    }

    PacketType       _type;
  };

  std::string& operator<<(std::string& a, const APacket& p);
  APacket& operator>>(const std::string& a, APacket& p);
  std::string& operator>>(const APacket& p, std::string& a);
  APacket& operator<<(APacket& p, const std::string& a);

};

#endif
