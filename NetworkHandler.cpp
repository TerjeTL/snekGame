#include "NetworkHandler.h"

NetworkHandler::NetworkHandler(sf::Mutex& mtx_, std::vector<Ghost>& ghosts_, const std::vector<Point>& points_): mtx(mtx_), ghosts(ghosts_), points(points_)

{
	//socket.setBlocking(false);
}

NetworkHandler::~NetworkHandler()

{
	if (recieveThread)

	{
		recieveThread->wait();
		delete recieveThread;
	}
}

void NetworkHandler::sendPos(Vec2f pos, Vec2f vel)

{
	if (posClock.getElapsedTime().asSeconds() > posRate)

	{
		MovePacket movePacket(pos.x, pos.y, 0.0, 0.0);
		sf::Packet packetSend;
		packetSend << movePacket;
		packetMtx.lock();
		threadPacket = packetSend;
		packetMtx.unlock();
		socket.send(packetSend, ip, port);
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
		//packetMtx.lock();
		//threadPacket = packetSend;
		//packetMtx.unlock();
		socket.send(packetSend, ip, port);
	}
}

void NetworkHandler::sendClear()

{
	ClearPacket clearPacket;
	sf::Packet packetSend;
	packetSend << clearPacket;
	//packetMtx.lock();
	//threadPacket = packetSend;
	//packetMtx.unlock();
	socket.send(packetSend, ip, port);
}

void NetworkHandler::sendCreate()

{
	CreateGhostPacket createGhostPacket;
	sf::Packet packetSend;
	packetSend << createGhostPacket;
	//packetMtx.lock();
	//threadPacket = packetSend;
	//packetMtx.unlock();
	socket.send(packetSend, ip, port);
	//socket.setBlocking(false);
}

void NetworkHandler::sendAlive()

{
	if (alive.getElapsedTime().asSeconds() > 1.0)

	{
		alive.restart();
		AlivePacket alivePacket;
		sf::Packet sendPacket;
		sendPacket << alivePacket;
		socketMtx.lock();
		socket.setBlocking(true);
		socket.send(sendPacket, ip, port);
		socketMtx.unlock();
	}
}

void NetworkHandler::send()

{
	while (!quit)

	{
		if (threadPacket.getDataSize() != 0)

		{
			socketMtx.lock();
			socket.setBlocking(true);
			int index = socket.send(threadPacket, ip, port);
			//std::cout << index << std::endl;
			socketMtx.unlock();
			threadPacket.clear();
		}
	}
}

void NetworkHandler::sendUpdateSnakes(std::string id)

{
	sf::Packet packetSend;
	CreateGhostPacket createGhostPacket;
	createGhostPacket.first = 0;
	packetSend << createGhostPacket;
	socketMtx.lock();
	socket.send(packetSend, ip, port);
	socketMtx.unlock();
	for (int i = 0; i < points.size(); i++)

	{
		packetSend.clear();
		//sendPoint(points[i], id);
		PointPacket pointPacket(points[i].position.x, points[i].position.y, points[i].type, points[i].radius);
		pointPacket.id = id;
		packetSend << pointPacket;
		socket.send(packetSend, ip, port);
	}
}

void NetworkHandler::receive()

{
	
	while (!quit)

	{
		
		
		sf::Packet packetRecieve;
		socketMtx.lock();
		socket.setBlocking(false);
		sf::IpAddress sender;
		unsigned short senderPort;
		int ready = (socket.receive(packetRecieve, sender, senderPort) == sf::Socket::Done);
		//std::cout << sender << ":" << senderPort << std::endl;
		socketMtx.unlock();
		if (ready)

		{
			
			unsigned char msg;
			packetRecieve >> msg;
			//std::cout << msg << std::endl;
			//mtx.lock();

			

			if (msg == MOVE)

			{
				MovePacket packet;
				packetRecieve >> packet;
				std::string id;
				packetRecieve >> id;
				mtx.lock();
				int index = findGhost(id);
				if (index != -1) ghosts[index].position.x = packet.x, ghosts[index].position.y = packet.y, ghosts[index].velocity.x = packet.velX, ghosts[index].velocity.y = packet.velY;
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
				if (index != -1 && (packet.id == myID || packet.id == "")) ghosts[index].points.push_back(Point(Vec2f(packet.x, packet.y), packet.type, packet.radius));
				mtx.unlock();
			}

			if (msg == CREA)

			{
				std::string id;
				CreateGhostPacket packet;
				packetRecieve >> packet;
				packetRecieve >> id;
				mtx.lock();
				std::cout << "Created ghost with " << id << std::endl;
				ghosts.push_back(Ghost(id));
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

			if (msg == MYID)

			{
				std::string id;
				packetRecieve >> id;
				myID = id;
				std::cout << "My ID " << id << std::endl;
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

			if (msg == ALIV)

			{
				clock.restart();
			}
			//mtx.unlock();
			packetRecieve.clear();
			//socketMtx.lock();
			//socket.setBlocking(false);
			//ready = (socket.receive(packetRecieve) == sf::Socket::Done);
			//socketMtx.unlock();
		}

		if (clock.getElapsedTime().asSeconds() > 10 && connected)

		{
			std::cout << "Lost connection to server" << std::endl;
			connected = 0;
			//socket.disconnect();
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

void NetworkHandler::connect(std::string ip_, int port_)

{
	//if (!socket.connect(ip, port, sf::seconds(5)))

	//{
	ip = ip_;
	port = port_;
	socket.bind(socket.AnyPort);
	std::cout << "Connected to " << ip << ":" << port << std::endl;
	//}
	sf::Packet packet;
	packet << CreateGhostPacket();
	socket.send(packet, ip, port);
	connected = 1;
	clock.restart();

	recieveThread = new sf::Thread(&NetworkHandler::receive, this);
	recieveThread->launch();
}

void NetworkHandler::quitConnection()

{
	quit = 1;
	socketMtx.lock();
	DisconnectPacket disconnectPacket;
	sf::Packet sendPacket;
	sendPacket << disconnectPacket;
	socket.send(sendPacket, ip, port);
	//socket.disconnect();
	socketMtx.unlock();
}