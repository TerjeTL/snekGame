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
	std::string id = "";
};

struct CreateGhostPacket

{
	CreateGhostPacket()

	{

	}

	char name[5] = "crea";
	int first = 1;
};

struct ClearPacket

{
	ClearPacket()

	{

	}

	char name[5] = "mclr";
};

struct myIDPacket

{
	myIDPacket()

	{

	}

	char name[5] = "myid";
};

struct DisconnectPacket

{
	DisconnectPacket()

	{

	}

	char name[5] = "dsct";
};

sf::Packet& operator <<(sf::Packet& packet, const MovePacket& move);
sf::Packet& operator >>(sf::Packet& packet, MovePacket& move);
sf::Packet& operator <<(sf::Packet& packet, const PointPacket& point);
sf::Packet& operator >>(sf::Packet& packet, PointPacket& point);
sf::Packet& operator <<(sf::Packet& packet, const CreateGhostPacket& create);
sf::Packet& operator >>(sf::Packet& packet, CreateGhostPacket& create);
sf::Packet& operator <<(sf::Packet& packet, const ClearPacket& clear);
sf::Packet& operator <<(sf::Packet& packet, const myIDPacket& idpacket);
sf::Packet& operator <<(sf::Packet& packet, const DisconnectPacket& disconnect);