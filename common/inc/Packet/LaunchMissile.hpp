#ifndef _LAUNCHMISSILE_HPP_
# define _LAUNCHMISSILE_HPP_

# include "Packet/APacket.hpp"

namespace Packet {
    class LaunchMissile : public APacket
    {
    public:
		LaunchMissile();
		LaunchMissile(uint16_t);

		virtual ~LaunchMissile() = default;

        uint16_t getPlayerId() const;

        virtual std::string to_bytesNoHeader() const override;
        virtual size_t from_bytes(const std::string &bytes) override;

    private:
        uint16_t _playerId;
    };
};

#endif