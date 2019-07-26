#pragma once
#include <vector>
#include <list>
#include <iostream>
#include <SFML/Network.hpp>
#include "Socket.h"
#include "../Packets.h"
#include "UdpClient.h"

class Program

{
public:

	Program();
	~Program();
	int mainLoop();
	void executeThread();
	void server();
	void client();
	void getInput();
	void checkTimeout();
	void disconnect(int index);
	void broadcast(sf::Packet& packet, int index = -1);
	void broadcastUDP(sf::Packet& packet, int index = -1, int me = 0);

	void generateID(std::string& id);
	int checkID(std::string &id);

private:
	sf::TcpSocket clientSocket;
	sf::Thread* thread = 0;
	sf::SocketSelector selector;
	//sf::UdpListener listener;
	std::vector<std::string> ids;
	sf::Mutex mtx;
	int quit = 0;
	std::string msgSend;
	sf::Clock alive;
	std::vector<Socket*> sockets;
	sf::UdpSocket serverSocket;
	std::vector<UdpClient> clients;
};