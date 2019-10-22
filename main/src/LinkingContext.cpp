#include "LinkingContext.hpp"
#include "game_object.hpp"
#include <optional>

using NetworkID = uint32_t;


LinkingContext::LinkingContext(std::map<NetworkID, GameObject*> idObjt, std::map<GameObject*, ReplicationClassID> pointObjt){

}

void LinkingContext::addGameObjectAndID(NetworkID id, GameObject obj)
{
	
	//std::pair<int, GameObject*>(1, &obj);

	idObjt.insert(std::pair<NetworkID, GameObject*>(id, &obj));
	pointObjt.insert(std::pair<GameObject*, NetworkID>(&obj, id));
}

void LinkingContext::deletePointer(GameObject* obj)
{
	std::map<GameObject*, NetworkID>::iterator deleted;

	deleted = pointObjt.find(obj);

	if (deleted != pointObjt.end())
		pointObjt.erase(deleted);
}

void LinkingContext::addGameObject(GameObject* obj)
{
	

	idObjt.insert(std::pair<NetworkID, GameObject*>(currentId, obj));
	pointObjt.insert(std::pair<GameObject*, NetworkID>(obj, currentId));

	currentId++;
}

std::optional<int> LinkingContext::getIDFromObject(GameObject* obj)
{

	const auto iterator = pointObjt.find(obj);
	if (iterator == pointObjt.end()) return std::nullopt;
	return std::make_optional<NetworkID>(iterator->second);
}

std::optional<GameObject*> LinkingContext::getObjectFromID(NetworkID id)
{
	const auto iterator = idObjt.find(id);
	if (iterator == idObjt.end()) return std::nullopt;
	return std::make_optional<GameObject*>(iterator->second);
}
