#include "Packets.h"

sf::Packet& operator <<(sf::Packet& packet, const MovePacket& move)

{
	return packet << move.name << move.x << move.y << move.velX << move.velY << move.bodySize << move.pointsAllowed;
}

sf::Packet& operator >>(sf::Packet& packet, MovePacket& move)

{
	return packet >> move.x >> move.y >> move.velX >> move.velY >> move.bodySize >> move.pointsAllowed;
}

sf::Packet& operator <<(sf::Packet& packet, const PointPacket& point)

{
	return packet << point.name << point.x << point.y << point.type << point.radius << point.id;
}

sf::Packet& operator >>(sf::Packet& packet, PointPacket& point)

{
	return packet >> point.x >> point.y >> point.type >> point.radius >> point.id;
}

sf::Packet& operator <<(sf::Packet& packet, const CreateGhostPacket& create)

{
	return packet << create.name << create.first << create.r << create.g << create.b;
}

sf::Packet& operator >>(sf::Packet& packet, CreateGhostPacket& create)

{
	return packet >> create.first >> create.r >> create.g >> create.b;
}

sf::Packet& operator <<(sf::Packet& packet, const ClearPacket& clear)

{
	return packet << clear.name;
}

sf::Packet& operator <<(sf::Packet& packet, const myIDPacket& idpacket)

{
	return packet << idpacket.name << idpacket.r << idpacket.g << idpacket.b;
}

sf::Packet& operator >>(sf::Packet& packet, myIDPacket& idpacket)

{
	return packet >> idpacket.r >> idpacket.g >> idpacket.b;
}

sf::Packet& operator <<(sf::Packet& packet, const DisconnectPacket& disconnect)

{
	return packet << disconnect.name;
}

sf::Packet& operator <<(sf::Packet& packet, const AlivePacket& alive)

{
	return packet << alive.name;
}

sf::Packet& operator <<(sf::Packet& packet, const UDPJoinPacket& udpj)

{
	return packet << udpj.name;
}

sf::Packet& operator <<(sf::Packet& packet, const DeadPacket& dead)

{
	return packet << dead.name;
}