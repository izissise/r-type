#ifndef _MOVEPACKET_HPP_
# define _MOVEPACKET_HPP_

# include "Packet/APacket.hpp"

namespace Packet {
    class MovePacket : public APacket
    {
    public:
        MovePacket();
        MovePacket(uint16_t, uint8_t, float);

        virtual ~MovePacket() = default;

        uint8_t getAxis() const;
        uint16_t getSpeed() const;
        uint16_t getPlayerId() const;

        virtual std::string to_bytesNoHeader() const override;
        virtual size_t from_bytes(const std::string &bytes) override;

    private:
        uint16_t _playerId;
        uint8_t _axis;
        float _speed;
    };
};

#endif