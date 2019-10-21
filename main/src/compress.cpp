#include <algorithm>
#include "compress.hpp"

namespace compression
{
	CompressedPosType PositionCompressor::operator()(const PositionType& value) const noexcept
	{
		CompressedPosType result;
		result.x = compressFloat(value.x);
		result.y = compressFloat(value.y);
		result.z = compressFloat(value.z);
		return result;
	}

	uint32_t PositionCompressor::compressFloat(float value) const noexcept
	{
		const auto compressed = (value * 1000) + 500000;
		return static_cast<uint32_t>(compressed);
	}

	CompressedQuatType QuaternionCompressor::operator()(const QuaternionType& value) const noexcept
	{
		const auto compressedX = compressFloat(value.x);
		const auto compressedY = compressFloat(value.y);
		const auto compressedZ = compressFloat(value.z);
		const auto compressedW = compressFloat(value.w);
		const auto max = std::max({ compressedX, compressedY, compressedZ, compressedW });

		CompressedQuatType result;
		if (max == compressedX)
		{
			result.missing = QuaternionParameterType::x;
			result.first = compressedY;
			result.second = compressedZ;
			result.third = compressedW;
		}
		else if (max == compressedY)
		{
			result.missing = QuaternionParameterType::y;
			result.first = compressedX;
			result.second = compressedZ;
			result.third = compressedW;
		}
		else if (max == compressedZ)
		{
			result.missing = QuaternionParameterType::z;
			result.first = compressedX;
			result.second = compressedY;
			result.third = compressedW;
		}
		else
		{
			result.missing = QuaternionParameterType::w;
			result.first = compressedX;
			result.second = compressedY;
			result.third = compressedZ;
		}

		return result;
	}

	uint16_t QuaternionCompressor::compressFloat(float value) const noexcept
	{
		const auto compressed = (value * 1000) + 1000;
		return static_cast<uint32_t>(compressed);
	}
}