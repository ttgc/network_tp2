#pragma once
#include <stdint.h>
#include <gsl/gsl>

namespace coordinates
{
	struct Position
	{
		float x;
		float y;
		float z;
	};

	struct Quaternion
	{
		float x;
		float y;
		float z;
		float w;
	};

	namespace compressed
	{
		struct Position
		{
			uint32_t x;
			uint32_t y;
			uint32_t z;
		};

		struct Quaternion
		{
			enum QuaternionParameter : uint8_t
			{
				x = 0b00,
				y = 0b01,
				z = 0b10,
				w = 0b11
			};

			QuaternionParameter missing;
			uint16_t first;
			uint16_t second;
			uint16_t third;
		};
	}
}

#ifndef NO_ALIAS
using PositionType = coordinates::Position;
using QuaternionType = coordinates::Quaternion;
using CompressedPosType = coordinates::compressed::Position;
using CompressedQuatType = coordinates::compressed::Quaternion;
using QuaternionParameterType = coordinates::compressed::Quaternion::QuaternionParameter;
#endif // !NO_ALIAS
