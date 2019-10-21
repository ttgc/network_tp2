#include "compress.hpp"

namespace compression
{
	CompressedPosType PositionCompressor::operator()(const PositionType& value) const noexcept
	{
		return CompressedPosType();
	}

	CompressedQuatType QuaternionCompressor::operator()(const QuaternionType& value) const noexcept
	{
		return CompressedQuatType();
	}
}