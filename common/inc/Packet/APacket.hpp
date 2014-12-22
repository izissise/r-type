#ifndef _APACKET_HPP_
# define _APACKET_HPP_

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
      ASKLISTROOM = 5, //5
      STARTGAME = 6, //6
      READYGAME = 7, //7
      LEAVEROOM = 8, //8
      MESSAGE = 9, //9
      GETLISTPLAYER = 10, //10
      UNKNOW,
    };

  class PackerParsingError : public std::runtime_error
  {
	public:
		PackerParsingError(const std::string& err) throw() : runtime_error(err) {};
		virtual ~PackerParsingError() throw() {};
  };

  public:
    APacket(Packet::APacket::PacketType type);
    virtual ~APacket() = default;

    operator std::string() const;

    Packet::APacket::PacketType getType() const {return _type;};

    std::string to_bytes() const;
    virtual std::string to_bytesNoHeader() const = 0;
    virtual size_t from_bytes(const std::string &bytes) = 0;

    static Packet::APacket::PacketType toPacketType(uint16_t p);
	static Packet::APacket::PacketType toPacketType(const std::string& buff);

	static uint16_t fromPacketType(Packet::APacket::PacketType p) {return static_cast<uint16_t>(p);};

    template <typename T>
    static void fill_bytes(std::string &bytes, T nb)
    {
      auto it = bytes.end();

      for (size_t i = 0; i < sizeof(T); ++i)
      {
        it = bytes.insert(it, (nb & static_cast<uint8_t>(0xFF)));
        nb = nb >> 8;
      }
    }

    template <typename T>
    static void get_bytes(const std::string &bytes, size_t &pos, T &nb)
    {
      size_t i;

      for (i = 0; i < sizeof(T) && pos + i != bytes.size(); ++i)
        nb = ((nb << 8) | static_cast<uint8_t>(bytes[pos + i]));
      pos += i;
      if (i < sizeof(T))
        throw APacket::PackerParsingError("Error while parsing packet");
    }
  protected:

    PacketType       _type;
  };

  std::string& operator<<(std::string& a, const APacket& p);
  APacket& operator>>(const std::string& a, APacket& p);
  std::string& operator>>(const APacket& p, std::string& a);
  APacket& operator<<(APacket& p, const std::string& a);

};

#endif
