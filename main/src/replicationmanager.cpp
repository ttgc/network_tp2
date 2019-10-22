#include "replicationmanager.hpp"
#include "classregistry.hpp"

utils::PacketType ReplicationManager::s_syncPacket = utils::PacketType::Sync;

void ReplicationManager::replicate(MemoryStream& stream, const std::vector<GameObject*> objects)
{
	stream.Flush();
	stream.Write<const uint32_t>(utils::PROTOCOL_ID);
	stream.Write<utils::PacketType>(s_syncPacket);

	OutputStream ostream;
	std::for_each(objects.begin(), objects.end(), [&ostream](GameObject* obj) {
		ostream.Write<ReplicationClassID>(obj->ClassID);
		//ostream.Write(networkID);
		obj->write(ostream);
	});
	stream.Write<const size_t>(ostream.Size());
	stream.Write(ostream.Read(ostream.Size()));
}

void ReplicationManager::replicate(MemoryStream& stream)
{
	if (!(stream.Read<const uint32_t>() == utils::PROTOCOL_ID)) return;
	if (!(stream.Read<utils::PacketType>() == s_syncPacket)) return;
	const auto registry = ClassRegistry::get();
	std::vector<GameObject*> streamContent;
	const size_t size = stream.Read<const size_t>();

	while (stream.Size() > 0)
	{
		const ReplicationClassID classID = stream.Read<ReplicationClassID>();
		//const networkID;
		GameObject* obj;
		//if (in linkingContext)
		{
			//context.get(networkID);
		}
		//else
		{
			obj = registry.lock()->create(classID);
			//insert obj in linkingContext
			m_replicated.insert(obj);
		}
		obj->read(reinterpret_cast<InputStream&>(stream));
		streamContent.push_back(obj);
	}

	const auto newend = std::remove_if(
		m_replicated.begin(),
		m_replicated.end(), 
		[&streamContent](GameObject* obj) -> bool {
			const auto pos = std::find_if(
				streamContent.begin(),
				streamContent.end(),
				[obj](GameObject* vecObj) -> bool { return vecObj == obj; }
			);
			const bool toRemove(pos == streamContent.end());
			if (toRemove) obj->destroy();
			return toRemove;
		}
	);
	m_replicated.erase(newend, m_replicated.end());
}
