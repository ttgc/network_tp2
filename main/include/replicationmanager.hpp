#pragma once

#include <unordered_set>
#include <vector>
#include "game_object.hpp"
#include "networkutils.hpp"
#include "streams.hpp"

class ReplicationManager
{
public:
	static std::weak_ptr<ReplicationManager> get();

	void replicate(MemoryStream& stream, const std::vector<GameObject*> objects);
	void replicate(MemoryStream& stream);

	std::vector<GameObject> getObjects();

private:
	ReplicationManager() = default;
	ReplicationManager(const ReplicationManager&) = delete;
	ReplicationManager& operator=(const ReplicationManager&) = delete;
	static std::shared_ptr<ReplicationManager> s_instance;

	std::unordered_set<GameObject*> m_replicated;
	static utils::PacketType s_syncPacket;
};