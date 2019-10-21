#pragma once
#include <map>
#include <functional>
#include <memory>
#include "game_object.hpp"

class ClassRegistry
{
public:
	static std::weak_ptr<ClassRegistry> get();

	template<typename T>
	void registerClass(std::function<GameObject* ()> generator)
	{
		static_assert(std::is_base_of<GameObject, T>::value, "T should be a GameObject");
		const auto pair = 
			std::pair<ReplicationClassID, std::function<GameObject* ()> >(T::mClassID, generator);
		m_generators.insert(pair);
	}

	GameObject* create(ReplicationClassID classID);

private:
	ClassRegistry() = default;
	ClassRegistry(const ClassRegistry&) = delete;
	ClassRegistry& operator=(const ClassRegistry&) = delete;
	static std::shared_ptr<ClassRegistry> s_instance;

	std::map<ReplicationClassID, std::function<GameObject* ()> > m_generators;
};
