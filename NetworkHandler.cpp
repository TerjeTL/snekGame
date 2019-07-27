#include "NetworkHandler.h"

NetworkHandler::NetworkHandler(sf::Mutex& mtx_, std::vector<Ghost>& ghosts_, const std::vector<Point>& points_, std::vector<Point>& foods_): mtx(mtx_), ghosts(ghosts_), points(points_),
receiveThreadUDP(&NetworkHandler::receiveUDP, this), receiveThreadTCP(&NetworkHandler::receiveTCP, this), foods(foods_)

{
	udpSocket.setBlocking(false);
}

NetworkHandler::~NetworkHandler()

{
	receiveThreadUDP.wait();
	receiveThreadTCP.wait();
}

void NetworkHandler::sendPos(Vec2f pos, Vec2f vel, int pointsAllowed, float bodySize)

{
	if (posClock.getElapsedTime().asSeconds() > posRate)

	{
		MovePacket movePacket(pos.x, pos.y, vel.x, vel.y, pointsAllowed, bodySize);
		sf::Packet packetSend;
		packetSend << movePacket;
		udpSocket.send(packetSend, ip, port);
		posClock.restart();
	}
}

void NetworkHandler::sendPoint(const Point& point, const std::string& id)

{
	if (pointClock.getElapsedTime().asSeconds() > pointRate)

	{
		PointPacket pointPacket(point.position.x, point.position.y, point.type, point.radius);
		sf::Packet packetSend;
		packetSend << pointPacket;
		udpSocket.send(packetSend, ip, port);
		pointClock.restart();
	}
}

void NetworkHandler::sendClear()

{
	ClearPacket clearPacket;
	sf::Packet packetSend;
	packetSend << clearPacket;
	tcpSocket.setBlocking(true);
	tcpSocket.send(packetSend);
	tcpSocket.setBlocking(false);
}

void NetworkHandler::sendCreate()

{
	CreateGhostPacket createGhostPacket(color.r, color.g, color.b);
	sf::Packet packetSend;
	packetSend << createGhostPacket;
	tcpSocket.send(packetSend);
}

void NetworkHandler::sendEat(int index)

{
	EatPacket eatPacket(index);
	sf::Packet packetSend;
	packetSend << eatPacket;
	tcpSocket.send(packetSend);
}

void NetworkHandler::sendUpdateSnakes(std::string id)

{
	sf::Packet packetSend;
	CreateGhostPacket createGhostPacket(color.r, color.g, color.b);
	createGhostPacket.first = 0;
	packetSend << createGhostPacket;
	socketMtxTCP.lock();
	tcpSocket.send(packetSend);
	socketMtxTCP.unlock();
	for (int i = 0; i < points.size(); i++)

	{
		packetSend.clear();
		PointPacket pointPacket(points[i].position.x, points[i].position.y, points[i].type, points[i].radius);
		pointPacket.id = id;
		packetSend << pointPacket;
		udpSocket.send(packetSend, ip, port);
	}
}

void NetworkHandler::receiveTCP()

{
	sf::Packet packetRecieve;
	while(!quitTCP)

	{
		packetRecieve.clear();
		//socketMtxTCP.lock();
		sf::Socket::Status type = tcpSocket.receive(packetRecieve);
		int ready = (type == sf::Socket::Done);
		//socketMtxTCP.unlock();

		if (type == sf::Socket::Disconnected && connected)

		{
			std::cout << "Disconnected from server!" << std::endl;
			connected = 0;
		}

		if (ready)

		{
			unsigned char msg;
			packetRecieve >> msg;

			if (msg == CREA)

			{
				std::string id;
				CreateGhostPacket packet;
				packetRecieve >> packet;
				packetRecieve >> id;
				mtx.lock();
				std::cout << "Created ghost with " << id << std::endl;
				ghosts.push_back(Ghost(id, packet.r, packet.g, packet.b));
				if (packet.first) sendUpdateSnakes(id);
				mtx.unlock();
			}

			if (msg == MCLR)

			{
				std::string id;
				packetRecieve >> id;
				mtx.lock();
				int index = findGhost(id);
				if (index != -1) ghosts[index].reset();
				mtx.unlock();
			}

			if (msg == NPNT)

			{
				PointPacket packet;
				packetRecieve >> packet;
				std::string id;
				packetRecieve >> id;

				mtx.lock();
				if (packet.type == -1)

				{
					foods.clear();
				}

				else

				{
					foods.push_back(Point(Vec2f(packet.x, packet.y), packet.type, packet.radius));
					int index = foods.size() - 1;
					foods[index].id = index;
					foods[index].serverID = index;
				}
				mtx.unlock();
			}

			if (msg == DSCT)

			{
				std::string id;
				packetRecieve >> id;
				mtx.lock();
				if (ghosts.size() == 1)

				{
					ghosts.clear();
				}

				else

				{
					int index = findGhost(id);
					if (index != -1) ghosts.erase(ghosts.begin() + index);
				}
				mtx.unlock();
			}
		}
		packetRecieve.clear();
	}
}

void NetworkHandler::receiveUDP()

{
	sf::Packet packetRecieve;
	while(!quitUDP)

	{
		packetRecieve.clear();
		socketMtxUDP.lock();
		sf::IpAddress sender;
		unsigned short senderPort;
		int ready = (udpSocket.receive(packetRecieve, sender, senderPort) == sf::Socket::Done);
		socketMtxUDP.unlock();

		if (ready)

		{
			unsigned char msg;
			packetRecieve >> msg;

			if (msg == MOVE)

			{
				MovePacket packet;
				packetRecieve >> packet;
				std::string id;
				packetRecieve >> id;
				mtx.lock();
				int index = findGhost(id);
				if (index != -1)
				{
					ghosts[index].serverPosition.x = packet.x, ghosts[index].serverPosition.y = packet.y, ghosts[index].velocity.x = packet.velX, ghosts[index].velocity.y = packet.velY, ghosts[index].bodySize = packet.bodySize;
					ghosts[index].correctPos();
					if (packet.pointsAllowed) ghosts[index].points.push_back(Point(Vec2f(packet.x, packet.y), SNAKE, packet.bodySize));
				}
				mtx.unlock();
			}

			if (msg == NPNT)

			{
				PointPacket packet;
				packetRecieve >> packet;
				std::string id;
				packetRecieve >> id;
				mtx.lock();
				int index = findGhost(id);
				if (index != -1 && (packet.id == myID || packet.id == ""))

				{
					if (packet.type == SNAKE)

					{
						ghosts[index].points.push_back(Point(Vec2f(packet.x, packet.y), packet.type, packet.radius));
					}
				}
				mtx.unlock();
			}
		}
	}
}

int NetworkHandler::findGhost(const std::string& id)

{
	for (int i = 0; i < ghosts.size(); i++)

	{
		if (ghosts[i].id == id)

		{
			return i;
			break;
		}
	}

	return -1;
}

void NetworkHandler::bindUDPSocket(unsigned short port)

{
	int code = udpSocket.bind(5000);
	int i = 0;
	while (code)

	{
		i++;
		code = udpSocket.bind(port + i);
	}

	std::cout << "UDP Socket bound to " << udpSocket.getLocalPort() << std::endl;
}

void NetworkHandler::connect(std::string ip_, int port_)

{
	ip = ip_;
	port = port_;

	if (!tcpSocket.connect(ip, port, sf::seconds(20)))

	{
		std::cout << "Connected to tcp on " << ip << ":" << port << std::endl;
	}

	bindUDPSocket(port);

	sf::Packet receivePacket;
	tcpSocket.receive(receivePacket);
	unsigned char msg;
	receivePacket >> msg;

	if (msg == MYID)

	{
		
		myIDPacket packet;
		receivePacket >> packet;
		color = sf::Color(packet.r, packet.g, packet.b);
		std::string id;
		receivePacket >> id;
		myID = id;
		std::cout << "My ID " << id << std::endl;
		std::cout << "Sending UDP connection packet!" << std::endl;
		sf::Packet sendPacket;
		UDPJoinPacket udpPacket;
		sendPacket << udpPacket;
		sendPacket << id;
		udpSocket.send(sendPacket, ip, port);
	}

	sf::Packet packet;
	packet << CreateGhostPacket(color.r, color.g, color.b);
	tcpSocket.send(packet);
	connected = 1;

	receiveThreadUDP.launch();
	receiveThreadTCP.launch();
}

void NetworkHandler::quitConnection()

{
	mtx.lock();
	quitTCP = 1;
	quitUDP = 1;
	mtx.unlock();

	//socketMtxTCP.lock();
	//tcpSocket.disconnect();
	//socketMtxTCP.unlock();
	socketMtxTCP.lock();
	DisconnectPacket disconnectPacket;
	sf::Packet sendPacket;
	sendPacket << disconnectPacket;
	tcpSocket.send(sendPacket);
	tcpSocket.disconnect();
	socketMtxTCP.unlock();
}