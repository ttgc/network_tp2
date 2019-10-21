#include "uncompress.hpp"
#include <cmath>

namespace compression
{
	PositionType PositionUncompressor::operator()(const CompressedPosType& value) const noexcept
	{
		PositionType result;
		result.x = uncompressFloat(value.x);
		result.y = uncompressFloat(value.y);
		result.z = uncompressFloat(value.z);
		return result;
	}

	float PositionUncompressor::uncompressFloat(uint32_t value) const noexcept
	{
		const auto uncompressed = static_cast<float>(value - 500000) / 1000.f;
		return uncompressed;
	}

	QuaternionType QuaternionUncompressor::operator()(const CompressedQuatType& value) const noexcept
	{
		QuaternionType result{ 0, 0, 0, 0 };
		const auto recalculatorCallback = 
			[](const float& val1, const float& val2, const float& val3) -> float {
				return std::sqrt(1 - (std::pow(val1, 2) + std::pow(val2, 2) + std::pow(val3, 2)));
			};

		switch (value.missing)
		{
		case QuaternionParameterType::x:
			result.y = value.first;
			result.z = value.second;
			result.w = value.third;
			result.x = recalculatorCallback(result.y, result.z, result.w);
			break;
		case QuaternionParameterType::y:
			result.x = value.first;
			result.z = value.second;
			result.w = value.third;
			result.y = recalculatorCallback(result.x, result.z, result.w);
			break;
		case QuaternionParameterType::z:
			result.x = value.first;
			result.y = value.second;
			result.w = value.third;
			result.z = recalculatorCallback(result.x, result.y, result.w);
			break;
		case QuaternionParameterType::w:
			result.x = value.first;
			result.y = value.second;
			result.z = value.third;
			result.w = recalculatorCallback(result.x, result.y, result.z);
			break;
		}

		return result;
	}

	float QuaternionUncompressor::uncompressFloat(uint16_t value) const noexcept
	{
		const auto uncompressed = static_cast<float>(value - 1000) / 1000.f;
		return uncompressed;
	}
}