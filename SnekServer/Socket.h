#pragma once
#include <SFML/Network.hpp>
struct Socket

{
	Socket()

	{

	}

	Socket(sf::TcpSocket* ptr): socket(ptr)

	{

	}

	~Socket()

	{
		delete socket;
	}

	std::string id;
	sf::TcpSocket* socket;
	//sf::IpAddress ip;
	//unsigned short port;
};