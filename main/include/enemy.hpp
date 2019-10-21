#pragma once

#include "game_object.hpp"

class Enemy : public GameObject
{
public:
	REPLICATED(2, Enemy);
};

