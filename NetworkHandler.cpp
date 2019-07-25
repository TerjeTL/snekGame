#include "NetworkHandler.h"

NetworkHandler::NetworkHandler(sf::Mutex& mtx_): mtx(mtx_)

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

void NetworkHandler::receive()

{
	while (!quit)

	{
		std::string msg;
		sf::Packet packetRecieve;
		if (socket.receive(packetRecieve) != sf::Socket::NotReady)

		{
			if ((packetRecieve >> msg) && !msg.empty())

			{
				mtx.lock(); 
				mtx.unlock();
			}
		}
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