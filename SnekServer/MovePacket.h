#pragma once
#include <SFML/Network.hpp>

struct MovePacket

{
	MovePacket()

	{

	}

	MovePacket(float x_, float y_, float velX_, float velY_): x(x_), y(y_), velX(velX_), velY(velY_)

	{

	}

	char name[5] = "move";
	float x, y, velX, velY;
};

sf::Packet& operator <<(sf::Packet& packet, const MovePacket& move)

{
	return packet << move.name << move.x << move.y << move.velX << move.velY;
}

sf::Packet& operator >>(sf::Packet& packet, MovePacket& move)

{
	return packet >> move.name >> move.x >> move.y >> move.velX >> move.velY;
}