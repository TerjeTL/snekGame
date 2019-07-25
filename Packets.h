#pragma once
#include <SFML/Network.hpp>

struct MovePacket

{
	MovePacket()

	{

	}

	MovePacket(float x_, float y_, float velX_, float velY_) : x(x_), y(y_), velX(velX_), velY(velY_)

	{

	}

	char name[5] = "move";
	float x, y, velX, velY;
};

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

sf::Packet& operator <<(sf::Packet& packet, const MovePacket& move);
sf::Packet& operator >>(sf::Packet& packet, MovePacket& move);
sf::Packet& operator <<(sf::Packet& packet, const PointPacket& point);
sf::Packet& operator >>(sf::Packet& packet, PointPacket& point);