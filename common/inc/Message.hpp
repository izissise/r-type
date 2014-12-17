#ifndef _MESSAGE_HPP_
# define _MESSAGE_HPP_

# include "APacket.hpp"

namespace Packet {
  class Message: public APacket
  {
  private:
    static uint16_t headerNumber;
    
  public:
    Message();
    Message(const std::string &);
    
    const std::string &getMsg() const;
    
  private:
    std::string to_bytes_body() const override;
    std::size_t from_bytes_body(const std::string &bytes) override;
    uint16_t getHeaderNumber() const override {return headerNumber;};
    
    std::string   _msg;
  };
};

#endif