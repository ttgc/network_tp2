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

	template <typename T>
	class ByteArrayCompressor : Compressor<T, gsl::span<std::byte>>
	{
	public:
		gsl::span<std::byte> operator()(const T& value) const noexcept override
		{
			static_assert(std::is_copy_constructible<T>::value &&
				(std::is_arithmetic<T>::value || std::is_enum<T>::value), "This compressor only support primitive data type as input");
			
			gsl::span<std::byte> array(sizeof(T));
			for (size_t i(0); i < sizeof(T); i++)
			{
				array[sizeof(T) - 1 - i] = (value >> (i * 8));
			}

			return array;
		}
	};
}