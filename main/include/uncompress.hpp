#pragma once
#include "coordinates.hpp"
#include <gsl/span>

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

	template <typename T>
	class ByteArrayUncompressor : Uncompressor<gsl::span<std::byte>, T>
	{
	public:
		T operator()(const gsl::span<std::byte>& value) const noexcept override
		{
			static_assert(std::is_copy_constructible<T>::value &&
				(std::is_arithmetic<T>::value || std::is_enum<T>::value), "This uncompressor only support primitive data type as output");
			static_assert(sizeof(T) == value.size(), "T should have the same size as the input value");

			T uncompressed;
			for (size_t i(0); i < value.size(); i++)
			{
				uncompressed |= (value[value.size() - 1 - i] << (i * 8));
			}

			return uncompressed;
		}
	};
}
