#pragma once

#include "game_object.hpp"
#include "coordinates.hpp"

class Enemy : public GameObject
{
public:
	REPLICATED(2, Enemy);
	void destroy() override;
	void write(OutputStream& stream) override;
	void read(InputStream& stream) override;

private:
	coordinates::Position m_position = { 0,0,0 };
	std::string m_type = "untyped";
	coordinates::Quaternion m_rotation = { 0,0,0,0 };
};

