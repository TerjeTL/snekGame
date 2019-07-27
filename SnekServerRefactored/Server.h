#pragma once
#include <iostream>
#include <vector>
#include "Client.h"
#include "../Packets.h"

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
private:
	std::vector<std::string> ids;
	std::vector<Client> clients;

	sf::SocketSelector selector;
	sf::TcpListener listener;
	sf::Clock ping;
	sf::UdpSocket udpSocket;

	unsigned int quit = 0;
};