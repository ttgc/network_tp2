#pragma once
#include "game_object.hpp"
#include <map>
#include <optional>

using NetworkID = uint32_t;

class LinkingContext
{
public:
	static std::weak_ptr<LinkingContext> get();

	void addGameObjectAndID(NetworkID id, GameObject* obj);

	void deletePointer(GameObject* obj);
	void addGameObject(GameObject* obj);

	std::optional<NetworkID> getIDFromObject(GameObject* obj);
	std::optional<GameObject*> getObjectFromID(NetworkID id);

private:
	LinkingContext() = default;
	LinkingContext(const LinkingContext&) = delete;
	LinkingContext& operator=(const LinkingContext&) = delete;
	static std::shared_ptr<LinkingContext> s_instance;

	std::map<NetworkID, GameObject*> idObjt;
	std::map<GameObject*, NetworkID> pointObjt;
};