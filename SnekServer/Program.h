#pragma once
#include <vector>
#include <list>
#include <iostream>
#include <SFML/Network.hpp>
#include "Socket.h"
#include "../Packets.h"

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
	void broadcast(sf::Packet& packet, int index);

	void generateID(std::string& id);
	int checkID(std::string &id);

private:
	sf::TcpSocket clientSocket;
	sf::Thread* thread = 0;
	sf::SocketSelector selector;
	sf::TcpListener listener;
	
	sf::Mutex mtx;
	int quit = 0;
	std::string msgSend;
	std::vector<Socket*> sockets;
};