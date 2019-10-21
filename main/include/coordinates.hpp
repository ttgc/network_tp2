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

		template <size_t MAX_SIZE>
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
