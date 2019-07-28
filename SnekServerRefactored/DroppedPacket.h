#pragma once
#include <SFML/Network.hpp>
struct DroppedPacket

{
	DroppedPacket(int index_, int me_, sf::Packet packet_) : index(index_), me(me_), packet(packet_)

	{

	}

	int index;
	int me;
	sf::Packet packet;
};