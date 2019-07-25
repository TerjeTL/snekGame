#pragma once
#include <iostream>
#include <string>
#include <SFML/Network.hpp>

class NetworkHandler

{
public:
	NetworkHandler(sf::Mutex& mtx_);
	~NetworkHandler();
	void receive();
	void connect(std::string ip, int port);
	void quitConnection();

private:
	sf::TcpSocket socket;
	std::string id;
	sf::Thread* recieveThread = 0;
	sf::Mutex& mtx;
	int quit = 0;
};