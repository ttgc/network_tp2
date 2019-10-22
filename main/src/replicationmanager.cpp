#include "replicationmanager.hpp"

utils::PacketType ReplicationManager::s_syncPacket = utils::PacketType::Sync;

void ReplicationManager::replicate(MemoryStream& stream, const std::vector<GameObject*> objects)
{
	stream.Flush();
	stream.Write<const uint32_t>(utils::PROTOCOL_ID);
	stream.Write<utils::PacketType>(s_syncPacket);

	OutputStream ostream;
	std::for_each(objects.begin(), objects.end(), [&ostream](GameObject* obj) {
		//obj->Write(ostream);
	});
	stream.Write<const size_t>(ostream.Size());
	stream.Write(ostream.Read(ostream.Size()));
}

void ReplicationManager::replicate(MemoryStream& stream)
{
}
