#pragma once
#include <SFML/Network.hpp>
#define MOVE 0
#define NPNT 1
#define CREA 2
#define MCLR 3
#define MYID 4
#define DSCT 5
#define ALIV 6
#define UDPJ 7
#define DEAD 8

struct MovePacket

{
	MovePacket()

	{

	}

	MovePacket(float x_, float y_, float velX_, float velY_, float pointsAllowed_, float bodySize_) : 
		x(x_), y(y_), velX(velX_), velY(velY_), pointsAllowed(pointsAllowed_), bodySize(bodySize_)

	{

	}

	unsigned char name = MOVE; //Move
	float x, y, velX, velY, bodySize;
	int pointsAllowed;
};

struct PointPacket

{
	PointPacket()

	{

	}

	PointPacket(float x_, float y_, int type_, float radius_) : x(x_), y(y_), type(type_), radius(radius_)

	{

	}

	unsigned char name = NPNT;
	int type;
	float x, y, radius;
	std::string id = "";
};

struct CreateGhostPacket

{
	CreateGhostPacket()

	{

	}

	unsigned char name = CREA;
	int first = 1;
	unsigned char r, g, b;
};

struct ClearPacket

{
	ClearPacket()

	{

	}

	unsigned char name = MCLR;
};

struct myIDPacket

{
	myIDPacket()

	{

	}

	unsigned char name = MYID;
	unsigned char r, g, b;
};

struct DisconnectPacket

{
	DisconnectPacket()

	{

	}

	unsigned char name = DSCT;
};

struct AlivePacket

{
	AlivePacket()

	{

	}

	unsigned char name = ALIV;
};

struct UDPJoinPacket

{
	UDPJoinPacket()

	{

	}

	unsigned char name = UDPJ;
};

struct DeadPacket

{
	DeadPacket()

	{

	}

	unsigned char name = DEAD;
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
sf::Packet& operator <<(sf::Packet& packet, const AlivePacket& alive);
sf::Packet& operator <<(sf::Packet& packet, const UDPJoinPacket& udpj);
sf::Packet& operator <<(sf::Packet& packet, const DeadPacket& dead);