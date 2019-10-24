#pragma once
#include "coordinates.hpp"
#include <gsl/gsl>

namespace compression
{
	USE_COORDINATES_ALIAS()

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

	private:
		uint32_t compressFloat(float value) const noexcept;
	};

	class QuaternionCompressor : Compressor<QuaternionType, CompressedQuatType>
	{
	public:
		CompressedQuatType operator()(const QuaternionType& value) const noexcept override;

	private:
		uint16_t compressFloat(float value) const noexcept;
	};
}