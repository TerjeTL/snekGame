#pragma once
#include <SFML/Network.hpp>

sf::Packet& operator <<(sf::Packet& packet, const PointPacket& move)

{
	//return packet << move.name << move.x << move.y << move.velX << move.velY;
}

sf::Packet& operator >>(sf::Packet& packet, PointPacket move)

{
	//return packet >> move.x >> move.y >> move.velX >> move.velY;
}

struct PointPacket

{
	PointPacket()

	{

	}

	PointPacket(float x_, float y_, int type_, float radius_) : x(x_), y(y_), type(type_), radius(radius_)

	{

	}

	char name[5] = "npnt";
	int type;
	float x, y, radius;
};