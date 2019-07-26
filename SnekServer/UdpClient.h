#pragma once
#include <SFML/Network.hpp>

struct UdpClient

{
	UdpClient()

	{

	}

	UdpClient(sf::IpAddress ip_, unsigned short port_, std::string id_): ip(ip_), port(port_), id(id_)

	{

	}

	std::string id;
	sf::IpAddress ip;
	sf::Clock clock;
	unsigned short port;
};