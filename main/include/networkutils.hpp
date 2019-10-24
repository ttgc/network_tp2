#pragma once

namespace utils
{
	constexpr uint8_t PROTOCOL_ID = 0xC0;
	constexpr size_t MAX_PACKET_SIZE = 1024;

	enum class PacketType : uint8_t
	{
		Hello = 0x00,
		Sync = 0x01,
		Bye = 0xFF
	};
}