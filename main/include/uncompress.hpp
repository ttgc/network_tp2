#pragma once
#include "coordinates.hpp"

namespace compression
{
	USE_COORDINATES_ALIAS()

	// Base class
	template<class InputType, class OutputType>
	class Uncompressor
	{
	public:
		virtual OutputType operator()(const InputType& value) const noexcept = 0;
	};

	// Derived classes
	class PositionUncompressor : Uncompressor<CompressedPosType, PositionType>
	{
	public:
		PositionType operator()(const CompressedPosType& value) const noexcept override;

	private:
		float uncompressFloat(uint32_t value) const noexcept;
	};

	class QuaternionUncompressor : Uncompressor<CompressedQuatType, QuaternionType>
	{
	public:
		QuaternionType operator()(const CompressedQuatType& value) const noexcept override;

	private:
		float uncompressFloat(uint16_t value) const noexcept;
	};
}
