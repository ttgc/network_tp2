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

	coordinates::Position getPosition() const { return m_position; }
	std::string getType() const { return m_type; }
	coordinates::Quaternion getRotation() const { return m_rotation; }

	void setPosition(const coordinates::Position& pos) { m_position = pos; }
	void setType(const std::string& type) { m_type = type; }
	void setRotation(const coordinates::Quaternion& rot) { m_rotation = rot; }

private:
	coordinates::Position m_position = { 0,0,0 };
	std::string m_type = "untyped";
	coordinates::Quaternion m_rotation = { 0,0,0,0 };
};

