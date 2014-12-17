#ifndef _MESSAGE_HPP_
# define _MESSAGE_HPP_

# include "APacket.hpp"

namespace Packet {
  class Message: public APacket
  {
  public:
    Message();
    Message(const std::string &);
    
    const std::string &getMsg() const;
    
  private:
    virtual std::string to_bytesNoHeader() const override;
    virtual size_t from_bytes(const std::string &bytes) override;
    
    std::string   _msg;
  };
};

#endif