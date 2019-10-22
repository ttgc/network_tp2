#pragma once
#include "coordinates.hpp"
#include "compress.hpp"
#include "uncompress.hpp"

namespace compression
{
	struct CompressorCollection
	{
		PositionCompressor posCompressor;
		QuaternionCompressor quaternionCompressor;
	};

	struct UncompressorCollection
	{
		PositionUncompressor posUncompressor;
		QuaternionUncompressor quaternionUncompressor;
	};
}
