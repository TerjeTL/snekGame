#pragma once
#include <SFML/Network.hpp>

class Client

{
public:
	Client()

	{

	}

	Client(sf::IpAddress ip_, unsigned short port_, std::string id_): ip(ip_), udpPort(port_), id(id_)

	{

	}

	~Client()

	{

	}

	void tcpSetup(std::string s)

	{
		tcpPort = socket->getRemotePort();
		ip = socket->getRemoteAddress();
		id = s;
	}

	//UDP Details
	std::string id;
	sf::IpAddress ip;
	unsigned short tcpPort;
	unsigned short udpPort = 0;
	sf::Clock clock;
	//TCP Socket
	std::shared_ptr<sf::TcpSocket> socket;
private:
};