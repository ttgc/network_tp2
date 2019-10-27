#include "replicationmanager.hpp"
#include "classregistry.hpp"
#include "LinkingContext.hpp"

utils::PacketType ReplicationManager::s_syncPacket = utils::PacketType::Sync;

void ReplicationManager::replicate(MemoryStream& stream, const std::vector<GameObject*> objects)
{
	stream.Flush();
	stream.Write<const uint32_t>(utils::PROTOCOL_ID);
	stream.Write<utils::PacketType>(s_syncPacket);

	const auto ctx = LinkingContext::get();

	MemoryStream ostream;
	auto _ = std::for_each(objects.begin(), objects.end(), [&ostream, ctx](GameObject* obj) {
		ReplicationClassID classID = obj->ClassID();
		ostream.Write<ReplicationClassID>(classID);
		const auto netid = ctx.lock()->getIDFromObject(obj);
		if (!netid.has_value()) ctx.lock()->addGameObject(obj);
		NetworkID networkID = netid.value_or(ctx.lock()->getIDFromObject(obj).value());
		ostream.Write<NetworkID>(networkID);
		obj->write(reinterpret_cast<OutputStream&>(ostream));
	});
	stream.Write(ostream.Read(ostream.Size()));
}

void ReplicationManager::replicate(MemoryStream& stream)
{
	if (stream.RemainingSize() < 2) return;
	if (stream.Read<uint32_t>() != utils::PROTOCOL_ID) return;
	if (stream.Read<utils::PacketType>() != s_syncPacket) return;
	const auto registry = ClassRegistry::get();
	const auto ctx = LinkingContext::get();
	std::vector<GameObject*> streamContent;

	while (stream.RemainingSize() > 0)
	{
		const ReplicationClassID classID = stream.Read<ReplicationClassID>();
		const NetworkID networkID = stream.Read<NetworkID>();
		const auto objFromContext = ctx.lock()->getObjectFromID(networkID);
		GameObject* obj = objFromContext.value_or(registry.lock()->create(classID));
		if (!objFromContext.has_value())
		{
			ctx.lock()->addGameObjectAndID(networkID, obj);
			m_replicated.insert(obj);
		}
		obj->read(reinterpret_cast<InputStream&>(stream));
		streamContent.push_back(obj);
	}

	std::vector<GameObject*> toRemove(m_replicated.size());
	std::remove_copy_if(
		m_replicated.begin(),
		m_replicated.end(), 
		toRemove.begin(),
		[&streamContent](GameObject* obj) -> bool {
			const auto pos = std::find_if(
				streamContent.begin(),
				streamContent.end(),
				[obj](GameObject* vecObj) -> bool { return vecObj == obj; }
			);
			return (pos != streamContent.end());
		}
	);
	auto _ = std::for_each(toRemove.begin(), toRemove.end(), [this, ctx](GameObject* obj) {
		m_replicated.erase(obj);
		ctx.lock()->deletePointer(obj);
		obj->destroy();
		delete obj;
	});
	stream.Flush();
}

std::vector<GameObject> ReplicationManager::getObjects()
{
	std::vector<GameObject> objects(m_replicated.size());
	std::transform(m_replicated.begin(), m_replicated.end(), objects.begin(), 
		[](GameObject* obj) -> GameObject { return GameObject(*obj); }
	);
	return objects;
}
