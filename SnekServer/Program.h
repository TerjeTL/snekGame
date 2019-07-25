#pragma once
#include <vector>
#include <list>
#include <iostream>
#include <SFML/Network.hpp>

class Program

{
public:

	Program();
	int mainLoop();
	void executeThread();
	void server();
	void client();
	void getInput();
	void broadcast(sf::Packet& packet, int index);

private:
	sf::TcpSocket clientSocket;
	sf::Thread* thread = 0;
	
	
	sf::Mutex mtx;
	int quit = 0;
	std::string msgSend;
	std::vector<sf::TcpSocket*> sockets;
};