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
	void sendPos(Vec2f pos, Vec2f vel, int pointsAllowed, float bodySize);
	void sendPoint(const Point& point, const std::string& id = "");
	void sendClear();
	void sendCreate();
	void sendAlive();
	void sendUpdateSnakes(std::string id);
	void connect(std::string ip, int port);
	void quitConnection();
	int findGhost(const std::string& id);

private:
	sf::UdpSocket socket;
	//sf::TcpSocket socket;
	sf::Packet threadPacket;
	std::string myID;
	sf::Thread recieveThread;
	sf::Mutex& mtx;
	sf::Mutex packetMtx;
	sf::Mutex socketMtx;
	std::vector<Ghost>& ghosts;
	const std::vector<Point>& points;
	sf::Clock clock;
	sf::Clock alive;
	int connected = 0;
	int quit = 0;

	sf::Clock posClock;
	float posRate = 1.0 / 60.0;
	sf::Clock pointClock;
	float pointRate = 1.0 / 40.0;

	sf::IpAddress ip = "127.0.0.1";
	int port = 5000;
};