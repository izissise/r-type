#ifndef _APACKET_HPP_
# define _APACKET_HPP_

# include <iostream>
# include <string>
# include <stdexcept>
# include <cstdint>

namespace Packet {
  
  class APacket
  {
  public:
    enum class PacketType
    {
      SHORTRESPONSE = 0,
      HANDSHAKE = 1,
      GETLISTROOM = 2,
      CREATEROOM = 3,
      JOINROOM = 4,
    };
    
  public:
    APacket(PacketType type);
    virtual ~APacket();
    
    const std::string to_bytes() const;
    void from_bytes(const std::string &bytes);
    
  protected:
    virtual const std::string to_bytes_body() const = 0;
    virtual void from_bytes_body(const std::string &bytes) = 0;
    
    template <typename T>
    void fill_bytes(std::string &bytes, T nb) const
    {
      auto it = bytes.end();
      
      
      for (std::size_t i = 0;i < sizeof(T);++i)
      {
        it = bytes.insert(it, (nb & 0xFF));
        nb = nb >> 8;
      }
    }
    
    template <typename T>
    void get_bytes(const std::string &bytes, std::size_t &pos, T &nb) const
    {
      std::size_t i = 0;
      
      for (;i < sizeof(T) && pos + i != bytes.size();++i)
        nb = ((nb << 8) | bytes[pos + i]);
      pos += i;
      if (pos < sizeof(T) + 1)
        throw std::invalid_argument("Error while parse packet");
    }
    
    uint8_t          _type;
  };
};

#endif
