#pragma once

#include <cstdint>
#include "streams.hpp"

using ReplicationClassID = uint32_t;

#define REPLICATED(id,className)                                    \
enum : ReplicationClassID{ mClassID = id };                         \
virtual ReplicationClassID ClassID() const { return mClassID; }     \
static GameObject* CreateInstance() { return new className(); }     \

class GameObject
{
public:
	REPLICATED(0, GameObject)
	virtual void destroy() {};
	virtual void write(OutputStream&) {};
	virtual void read(InputStream&) {};
};