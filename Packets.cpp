#include "Packets.h"

sf::Packet& operator <<(sf::Packet& packet, const MovePacket& move)

{
	return packet << move.name << move.x << move.y << move.velX << move.velY;
}

sf::Packet& operator >>(sf::Packet& packet, MovePacket& move)

{
	return packet >> move.x >> move.y >> move.velX >> move.velY;
}

sf::Packet& operator <<(sf::Packet& packet, const PointPacket& point)

{
	return packet << point.name << point.x << point.y << point.type << point.radius;
}

sf::Packet& operator >>(sf::Packet& packet, PointPacket& point)

{
	return packet >> point.x >> point.y >> point.type >> point.radius;
}