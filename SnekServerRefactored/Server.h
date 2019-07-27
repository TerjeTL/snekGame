#pragma once
#include <iostream>
#include <vector>
#include "Client.h"
#include "../Packets.h"
#include "../Point.h"

class Server

{
public:
	Server();
	int mainLoop();
	void broadcastTCP(sf::Packet packet, int index = -1, int me = 0);
	void broadcastUDP(sf::Packet packet, int index = -1, int me = 0);
	void generateID(std::string& s);
	void createID(std::string& s);
	int getUDPClient(const sf::IpAddress& ip, const unsigned short& port);
	int checkID(std::string& s);
	int randomInt(int min, int max);
private:
	std::vector<std::string> ids;
	std::vector<Client> clients;

	sf::SocketSelector selector;
	sf::TcpListener listener;
	sf::Clock ping;
	sf::UdpSocket udpSocket;
	unsigned int quit = 0;


	std::vector<Point> foods;
	std::vector<std::vector<int>> colors = 
	{ {0, 255, 0},  {255, 0, 0}, {255, 0, 255}, {0, 255, 255}, 
	{255, 160, 60}, {130, 60, 200}, {255, 255, 0}, {0, 0, 255}, 
	{100, 100, 100}, {0, 255, 155} };
};