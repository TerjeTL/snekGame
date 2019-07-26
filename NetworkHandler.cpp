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

	if (sendThread)

	{
		sendThread->wait();
		delete sendThread;
	}
}

void NetworkHandler::sendPos(Vec2f pos, Vec2f vel)

{
	MovePacket movePacket(pos.x, pos.y, 0.0, 0.0);
	sf::Packet packetSend;
	packetSend << movePacket;
	packetMtx.lock();
	threadPacket = packetSend;
	packetMtx.unlock();
	//socket.send(packetSend);
}

void NetworkHandler::sendPoint(const Point& point)

{
	PointPacket pointPacket(point.position.x, point.position.y, point.type, point.radius);
	sf::Packet packetSend;
	packetSend << pointPacket;
	packetMtx.lock();
	threadPacket = packetSend;
	packetMtx.unlock();
	//socket.send(packetSend);
}

void NetworkHandler::sendClear()

{
	ClearPacket clearPacket;
	sf::Packet packetSend;
	packetSend << clearPacket;
	packetMtx.lock();
	threadPacket = packetSend;
	packetMtx.unlock();
	//socket.send(packetSend);
}

void NetworkHandler::sendCreate()

{
	CreateGhostPacket createGhostPacket;
	sf::Packet packetSend;
	packetSend << createGhostPacket;
	packetMtx.lock();
	threadPacket = packetSend;
	packetMtx.unlock();
	//socket.send(packetSend);
	//socket.setBlocking(false);
}

void NetworkHandler::send()

{
	while (!quit)

	{
		if (threadPacket.getDataSize() != 0)

		{
			std::cout << "Sending packet" << std::endl;
			//socketMtx.lock();
			socket.send(threadPacket);
			//socketMtx.unlock();
			threadPacket.clear();
			std::cout << "Done sending packet" << std::endl;
		}
	}
}

void NetworkHandler::sendUpdateSnakes(std::string id)

{
	sf::Packet packetSend;
	CreateGhostPacket createGhostPacket;
	createGhostPacket.first = 0;
	packetSend << createGhostPacket;
	socket.send(packetSend);
	for (int i = 0; i < points.size(); i++)

	{
		packetSend.clear();
		PointPacket pointPacket(points[i].position.x, points[i].position.y, points[i].type, points[i].radius);
		pointPacket.id = id;
		packetSend << pointPacket;
		socket.send(packetSend);
	}
}

void NetworkHandler::receive()

{
	
	while (!quit)

	{

		unsigned char msg;
		sf::Packet packetRecieve;
		//socketMtx.lock();
		int ready = (socket.receive(packetRecieve) != sf::Socket::NotReady);
		//socketMtx.unlock();
		if (ready)

		{
			
			packetRecieve >> msg;
			//std::cout << msg << std::endl;

			if (msg == MOVE)

			{
				
				MovePacket packet;
				packetRecieve >> packet;
				std::string id;
				packetRecieve >> id;
				int index = findGhost(id);
				if (index != -1) ghosts[index].position.x = packet.x, ghosts[index].position.y = packet.y, ghosts[index].velocity.x = packet.velX, ghosts[index].velocity.y = packet.velY;
			}

			if (msg == NPNT)

			{
				PointPacket packet;
				packetRecieve >> packet;
				std::string id;
				packetRecieve >> id;
				int index = findGhost(id);
				if (index != -1 && (packet.id == myID || packet.id == "")) ghosts[index].points.push_back(Point(Vec2f(packet.x, packet.y), packet.type, packet.radius));
			}

			if (msg == CREA)

			{
				std::string id;
				CreateGhostPacket packet;
				packetRecieve >> packet;
				packetRecieve >> id;
				std::cout << "Created ghost with " << id << std::endl;
				ghosts.push_back(Ghost(id));
				if (packet.first) sendUpdateSnakes(id);
			}

			if (msg == MCLR)

			{
				std::string id;
				packetRecieve >> id;
				int index = findGhost(id);
				if (index != -1) ghosts[index].reset();
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
				if (ghosts.size() == 1)

				{
					ghosts.clear();
				}

				else

				{
					int index = findGhost(id);
					if (index != -1) ghosts.erase(ghosts.begin() + index);
				}
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

void NetworkHandler::connect(std::string ip, int port)

{
	if (socket.connect(ip, port))

	{
		std::cout << "Connected to " << ip << ":" << port << std::endl;
	}

	recieveThread = new sf::Thread(&NetworkHandler::receive, this);
	recieveThread->launch();

	sendThread = new sf::Thread(&NetworkHandler::send, this);
	sendThread->launch();
}

void NetworkHandler::quitConnection()

{
	quit = 1;
}