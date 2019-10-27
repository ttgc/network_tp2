#pragma once

#include "game_object.hpp"
#include "coordinates.hpp"

class Player : public GameObject
{
public:
	REPLICATED(1, Player);
	void destroy() override;
	void write(OutputStream& stream) override;
	void read(InputStream& stream) override;

	coordinates::Position getPosition() const { return m_position; }
	std::string getName() const { return m_name; }
	coordinates::Quaternion getRotation() const { return m_rotation; }

	void setPosition(const coordinates::Position& pos) { m_position = pos; }
	void setName(const std::string& name) { m_name = name; }
	void setRotation(const coordinates::Quaternion& rot) { m_rotation = rot; }

private:
	coordinates::Position m_position = { 0,0,0 };
	std::string m_name = "anonymous";
	coordinates::Quaternion m_rotation = { 0,0,0,0 };
};

