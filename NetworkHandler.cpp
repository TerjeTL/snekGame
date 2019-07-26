#include "NetworkHandler.h"

NetworkHandler::NetworkHandler(sf::Mutex& mtx_, std::vector<Ghost>& ghosts_, const std::vector<Point>& points_): mtx(mtx_), ghosts(ghosts_), points(points_)

{

}

NetworkHandler::~NetworkHandler()

{
	if (recieveThread)

	{
		recieveThread->wait();
		delete recieveThread;
	}
}

void NetworkHandler::sendPos(Vec2f pos)

{
	MovePacket movePacket(pos.x, pos.y, 0, 0);
	sf::Packet packetSend;
	packetSend << movePacket;
	socket.send(packetSend);
}

void NetworkHandler::sendPoint(const Point& point)

{
	PointPacket pointPacket(point.position.x, point.position.y, point.type, point.radius);
	sf::Packet packetSend;
	packetSend << pointPacket;
	socket.send(packetSend);
}

void NetworkHandler::sendClear()

{
	ClearPacket clearPacket;
	sf::Packet packetSend;
	packetSend << clearPacket;
	socket.send(packetSend);
}

void NetworkHandler::sendCreate()

{
	CreateGhostPacket createGhostPacket;
	sf::Packet packetSend;
	packetSend << createGhostPacket;
	socket.send(packetSend);
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
	socket.setBlocking(false);
	while (!quit)

	{

		std::string msg;
		sf::Packet packetRecieve;
		mtx.lock();
		int ready = (socket.receive(packetRecieve) != sf::Socket::NotReady);
		mtx.unlock();
		if (ready)

		{
			
			packetRecieve >> msg;
			//std::cout << msg << std::endl;

			if (msg == "move")

			{
				
				MovePacket packet;
				packetRecieve >> packet;
				std::string id;
				packetRecieve >> id;
				mtx.lock();
				int index = findGhost(id);
				if (index != -1) ghosts[index].position.x = packet.x, ghosts[index].position.y = packet.y;
				mtx.unlock();
			}

			if (msg == "npnt")

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

			if (msg == "crea")

			{
				std::string id;
				CreateGhostPacket packet;
				packetRecieve >> packet;
				packetRecieve >> id;
				std::cout << "Created ghost with " << id << std::endl;
				mtx.lock();
				ghosts.push_back(Ghost(id));
				if (packet.first) sendUpdateSnakes(id);
				mtx.unlock();
			}

			if (msg == "mclr")

			{
				std::string id;
				packetRecieve >> id;
				mtx.lock();
				int index = findGhost(id);
				if (index != -1) ghosts[index].reset();
				mtx.unlock();
			}

			if (msg == "myid")

			{
				std::string id;
				packetRecieve >> id;
				mtx.lock();
				myID = id;
				std::cout << "My ID " << id << std::endl;
				mtx.unlock();
			}

			if (msg == "dsct")

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
}

void NetworkHandler::quitConnection()

{
	quit = 1;
}