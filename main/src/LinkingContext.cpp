#include "LinkingContext.hpp"
#include "game_object.hpp"
#include <optional>
#include <algorithm>

using NetworkID = uint32_t;


LinkingContext::LinkingContext() 
	: currentId(0)
	, idObjt()
	, pointObjt()
{

}

void LinkingContext::addGameObjectAndID(NetworkID id, GameObject* obj)
{

	idObjt.insert(std::pair<NetworkID, GameObject*>(id, obj));
	pointObjt.insert(std::pair<GameObject*, NetworkID>(obj, id));
}

void LinkingContext::deletePointer(GameObject* obj)
{
	NetworkID id;
	const auto deletedObject = pointObjt.find(obj);

	if (deletedObject != pointObjt.end())
	{
		id = deletedObject->second;
		pointObjt.erase(deletedObject);
	}
	const auto deletedId = idObjt.find(id);

	if (deletedId != idObjt.end())
		idObjt.erase(deletedId);
}

void LinkingContext::addGameObject(GameObject* obj)
{

	NetworkID id = std::max_element(idObjt.begin(), idObjt.end(), [](const std::pair<NetworkID, GameObject*> a, const std::pair<NetworkID, GameObject*> b)
		{
			return a.first < b.first;
		})->first + 1;

	idObjt.insert(std::pair<NetworkID, GameObject*>(id, obj));
	pointObjt.insert(std::pair<GameObject*, NetworkID>(obj, id));
}

std::optional<NetworkID> LinkingContext::getIDFromObject(GameObject* obj)
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
