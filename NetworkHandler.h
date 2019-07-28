#pragma once
#include <iostream>
#include <string>
#include <Maths.h>
#include <list>
#include "Packets.h"
#include "Point.h"
#include "Ghost.h"
#include "map.h"
#include "program.h"

class Program;

class NetworkHandler

{
public:
	NetworkHandler(sf::Mutex& mtx_, std::vector<Ghost>& ghosts_, const std::vector<Point>& points_, std::vector<Point>& foods_, std::string& name_, Program*& program_);
	~NetworkHandler();
	void receiveUDP();
	void receiveTCP();
	void sendPos(Vec2f pos, Vec2f vel, int pointsAllowed, float bodySize);
	void sendPoint(const Point& point, const std::string& id = "");
	void sendClear();
	void sendCreate();
	void sendReady();
	void sendEat(int index);
	void sendUpdateSnakes(std::string id);
	void sendPacket(sf::Packet& packet);
	void sendDroppedPacket();
	void connect(std::string ip, int port);
	void quitConnection();
	void bindUDPSocket(unsigned short port);
	//void ptrSet(std::shared_ptr<Program> ptr);
	int findGhost(const std::string& id);
	sf::Color color;

private:
	sf::UdpSocket udpSocket;
	sf::TcpSocket tcpSocket;
	sf::Packet threadPacket;
	std::string myID;
	sf::Thread receiveThreadUDP;
	sf::Thread receiveThreadTCP;
	sf::Mutex& mtx;
	sf::Mutex packetMtx;
	sf::Mutex socketMtxUDP;
	sf::Mutex socketMtxTCP;
	std::vector<Ghost>& ghosts;
	std::vector<Point>& foods;
	std::string& name;
	const std::vector<Point>& points;
	int connected = 0;
	int quitTCP = 0;
	int quitUDP = 0;

	sf::Clock posClock;
	float posRate = 1.0 / 60.0;
	sf::Clock pointClock;
	float pointRate = 1.0 / 40.0;

	std::list<sf::Packet> droppedPackets;
	sf::IpAddress ip = "127.0.0.1";
	int port = 5000;

	Program*& program;
};