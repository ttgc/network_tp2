#include "classregistry.hpp"

std::shared_ptr<ClassRegistry> ClassRegistry::s_instance = nullptr;

std::weak_ptr<ClassRegistry> ClassRegistry::get()
{
	class ClassRegistryInitializer : public ClassRegistry {};
	if (s_instance == nullptr) s_instance = std::make_shared<ClassRegistryInitializer>();
	return s_instance;
}

inline GameObject* ClassRegistry::create(ReplicationClassID classID)
{
	return m_generators.find(classID) != m_generators.end() ? m_generators.at(classID)() : nullptr;
}
