#include "Packet/LaunchMissile.hpp"

namespace Packet {
	LaunchMissile::LaunchMissile() : APacket(Packet::APacket::PacketType::LAUNCHMISSILE)
	{

	}

	LaunchMissile::LaunchMissile(uint16_t playerId) : APacket(Packet::APacket::PacketType::LAUNCHMISSILE), _playerId(playerId)
	{

	}

	uint16_t LaunchMissile::getPlayerId() const
	{
		return (_playerId);
	}

	std::string LaunchMissile::to_bytesNoHeader() const
	{
		std::string ret("");

		fill_bytes(ret, _playerId);
		return (ret);
	}

	size_t LaunchMissile::from_bytes(const std::string &bytes)
	{
		std::size_t pos = 0;

		get_bytes(bytes, pos, _playerId);
		return pos;
	}
};