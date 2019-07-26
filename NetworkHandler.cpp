#include "NetworkHandler.h"

NetworkHandler::NetworkHandler(sf::Mutex& mtx_, std::vector<Ghost>& ghosts_): mtx(mtx_), ghosts(ghosts_)

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

void NetworkHandler::receive()

{
	while (!quit)

	{
		std::string msg;
		sf::Packet packetRecieve;
		mtx.lock();
		if (socket.receive(packetRecieve) != sf::Socket::NotReady)

		{
			
			packetRecieve >> msg;
			//std::cout << msg << std::endl;

			if (msg == "move")

			{
				
				MovePacket packet;
				packetRecieve >> packet;
				ghosts[0].position.x = packet.x;
				ghosts[0].position.y = packet.y;
			}

			if (msg == "npnt")

			{
				PointPacket packet;
				packetRecieve >> packet;
				ghosts[0].points.push_back(Point(Vec2f(packet.x, packet.y), packet.type, packet.radius));
			}
		}
		mtx.unlock();
	}
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