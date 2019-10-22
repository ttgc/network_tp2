#pragma once
#include "game_object.hpp"
#include <map>
#include <optional>

using NetworkID = uint32_t;

class LinkingContext
{
public:

	//Modifier int en Id GameObject

	std::map<NetworkID, GameObject*> idObjt;
	std::map<GameObject*, NetworkID> pointObjt;

	LinkingContext(std::map<NetworkID, GameObject*> idObjt, std::map<GameObject*, ReplicationClassID> pointObjt);

	void addGameObjectAndID(NetworkID id, GameObject obj);

	void deletePointer(GameObject* obj);
	void addGameObject(GameObject* obj);

	std::optional<int> getIDFromObject(GameObject* obj);
	std::optional<GameObject*> getObjectFromID(NetworkID id);

private:
	NetworkID currentId = 0;
	
};