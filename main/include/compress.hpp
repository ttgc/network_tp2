#pragma once
#include "coordinates.hpp"

namespace compression
{
	// Base class
	template<class InputType, class OutputType>
	class Compressor
	{
	public:
		virtual OutputType operator()(const InputType& value) const noexcept = 0;
	};

	// Derived classes
	class PositionCompressor : Compressor<PositionType, CompressedPosType>
	{
	public:
		CompressedPosType operator()(const PositionType& value) const noexcept override;
	};

	class QuaternionCompressor : Compressor<QuaternionType, CompressedQuatType>
	{
	public:
		CompressedQuatType operator()(const QuaternionType& value) const noexcept override;
	};
}