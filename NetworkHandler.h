#pragma once
#include <iostream>
#include <string>
#include <Maths.h>
#include "Packets.h"
#include "Point.h"
#include "Ghost.h"

class NetworkHandler

{
public:
	NetworkHandler(sf::Mutex& mtx_, std::vector<Ghost>& ghosts_);
	~NetworkHandler();
	void receive();
	void sendPos(Vec2f pos);
	void sendPoint(const Point& point);
	void connect(std::string ip, int port);
	void quitConnection();
	

private:
	sf::TcpSocket socket;
	std::string id;
	sf::Thread* recieveThread = 0;
	sf::Mutex& mtx;
	std::vector<Ghost>& ghosts;
	int quit = 0;
};