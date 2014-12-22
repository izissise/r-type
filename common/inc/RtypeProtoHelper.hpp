#ifndef RTYPEPROTOHELPER_H
#define RTYPEPROTOHELPER_H

#include <memory>
#include <string>
#include <map>
#include <iostream>

#include "Packet/APacket.hpp"
#include "ClientHelper.hpp"
#include "ABasicSocket.hpp"

template<typename T>
class RtypeProtoHelper : public Network::SocketClientHelper
{
protected:
  static std::map<Packet::APacket::PacketType, size_t (T::*)(const Network::Buffer&)> _netWorkBinds;

public:
  RtypeProtoHelper(const std::shared_ptr<Network::ABasicSocket>& sock)
    : SocketClientHelper(sock)
  {};

  RtypeProtoHelper()
  {};

  virtual ~RtypeProtoHelper() = default;

protected:
  void onRead(size_t readSize) override
  {
    const size_t headerSize = sizeof(uint16_t);
    bool		incomplete = false;
    Network::Buffer buff;
    Packet::APacket::PacketType pack;

    if (readSize == 0)
      return;
    while (_readBuff.getLeftRead() >= headerSize && !incomplete)
      {
        _readBuff.readBuffer(buff, headerSize);
        pack = Packet::APacket::toPacketType(buff);
        if (pack != Packet::APacket::PacketType::UNKNOW)
          {
            buff.clear();
            _readBuff.readBuffer(buff, _readBuff.getLeftRead());
            try {
                auto meth = _netWorkBinds.at(pack);
                try {
                    size_t nbUsed = ((static_cast<T*>(this))->*meth)(buff);
                    _readBuff.rollbackReadBuffer(buff.size() - nbUsed);
                  }
                catch (Packet::APacket::PackerParsingError& e)
                  {
                    incomplete = true;
                    std::cout << "RollBack of: " << buff.size() + headerSize << std::endl;
                    _readBuff.rollbackReadBuffer(buff.size() + headerSize);
                  }
              }
            catch (std::out_of_range& e)
              {
                _readBuff.rollbackReadBuffer(headerSize - 1);
              }
          }
        else
          {
            _readBuff.rollbackReadBuffer(headerSize - 1);
            std::cerr << "Received Unknown Packet" << std::endl;
          }
      }
  };
  void onWrite(size_t) override
  {};

  virtual void onDisconnet() override
  {};
};

#endif // RTYPEPROTOHELPER_H
