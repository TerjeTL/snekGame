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
	NetworkHandler(sf::Mutex& mtx_, std::vector<Ghost>& ghosts_, const std::vector<Point>& points_);
	~NetworkHandler();
	void receive();
	void send();
	void sendPos(Vec2f pos, Vec2f vel);
	void sendPoint(const Point& point);
	void sendClear();
	void sendCreate();
	void sendUpdateSnakes(std::string id);
	void connect(std::string ip, int port);
	void quitConnection();
	int findGhost(const std::string& id);

private:
	//sf::UdpSocket udpSocket;
	sf::TcpSocket socket;
	sf::Packet threadPacket;
	std::string myID;
	sf::Thread* recieveThread = 0;
	sf::Thread* sendThread = 0;
	sf::Mutex& mtx;
	sf::Mutex packetMtx;
	sf::Mutex socketMtx;
	std::vector<Ghost>& ghosts;
	const std::vector<Point>& points;
	sf::Clock clock;
	int connected = 0;
	int quit = 0;
};