#ifndef _MESSAGE_HPP_
# define _MESSAGE_HPP_

# include "APacket.hpp"

namespace Packet {
  class Message: public APacket
  {
  public:
    Message();
    Message(const std::string &msg);

    virtual ~Message() = default;

    const std::string &getMsg() const;

    virtual std::string to_bytesNoHeader() const override;
    virtual size_t from_bytes(const std::string &bytes) override;

  private:
    std::string   _msg;
  };
};

#endif