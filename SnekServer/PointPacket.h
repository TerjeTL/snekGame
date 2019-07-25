#pragma once
#include <SFML/Network.hpp>

sf::Packet& operator <<(sf::Packet& packet, const PointPacket& point)

{
	return packet << point.name << point.x << point.y << point.type << point.radius;
}

sf::Packet& operator >>(sf::Packet& packet, PointPacket point)

{
	return packet >> point.x >> point.y >> point.type >> point.radius;
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