#include "LinkingContext.hpp"
#include "game_object.hpp"
#include <optional>
#include <algorithm>

std::shared_ptr<LinkingContext> LinkingContext::s_instance = nullptr;

std::weak_ptr<LinkingContext> LinkingContext::get()
{
	class LinkingContextInitializer : public LinkingContext {};
	if (s_instance == nullptr) s_instance = std::make_shared<LinkingContextInitializer>();
	return s_instance;
}

void LinkingContext::addGameObjectAndID(NetworkID id, GameObject* obj)
{

	idObjt.insert(std::pair<NetworkID, GameObject*>(id, obj));
	pointObjt.insert(std::pair<GameObject*, NetworkID>(obj, id));
}

void LinkingContext::deletePointer(GameObject* obj)
{
	const auto deletedObject = pointObjt.find(obj);

	if (deletedObject != pointObjt.end())
	{
		NetworkID id = deletedObject->second;
		pointObjt.erase(deletedObject);
		const auto deletedId = idObjt.find(id);

		if (deletedId != idObjt.end())
			idObjt.erase(deletedId);
	}
	
}

void LinkingContext::addGameObject(GameObject* obj)
{

	const auto iterator = std::max_element(idObjt.begin(), idObjt.end(), [](const std::pair<NetworkID, GameObject*> a, const std::pair<NetworkID, GameObject*> b)
		{
			return a.first < b.first;
		});

	NetworkID id = iterator != idObjt.end() ? iterator->first + 1 : 0;

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
