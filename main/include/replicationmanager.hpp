#pragma once

#include <unordered_set>
#include <vector>
#include "game_object.hpp"
#include "networkutils.hpp"
#include "streams.hpp"

class ReplicationManager
{
public:
	void replicate(MemoryStream& stream, const std::vector<GameObject*> objects);
	void replicate(MemoryStream& stream);

private:
	std::unordered_set<GameObject*> m_replicated;
	static utils::PacketType s_syncPacket;
};