#include "enemy.hpp"
#include "compress_collections.hpp"

void Enemy::destroy()
{
}

void Enemy::write(OutputStream& stream)
{
	compression::CompressorCollection compressors;
	auto compressedPosition = compressors.posCompressor(m_position);
	auto compressedRotation = compressors.quaternionCompressor(m_rotation);
	stream.WriteStr(m_type);
	stream.Write<uint32_t>(compressedPosition.x);
	stream.Write<uint32_t>(compressedPosition.y);
	stream.Write<uint32_t>(compressedPosition.z);
	stream.Write<coordinates::compressed::Quaternion::QuaternionParameter>(compressedRotation.missing);
	stream.Write<uint16_t>(compressedRotation.first);
	stream.Write<uint16_t>(compressedRotation.second);
	stream.Write<uint16_t>(compressedRotation.third);
}

void Enemy::read(InputStream& stream)
{
}
