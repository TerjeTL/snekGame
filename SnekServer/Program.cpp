#include "Program.h"

Program::Program()

{

}

int Program::mainLoop()

{
	

	char who;
	std::cout << "Server (s) or client (c)?";
	std::cin >> who;

	if (who == 's')

	{
		server();
	}

	else
		
	{
		client();
	}

	return EXIT_SUCCESS;
}

void Program::server()

{
	sf::SocketSelector selector;
	sf::TcpListener listener;
	listener.listen(5000);
	selector.add(listener);
	
	
	while (!quit)

	{
		if (selector.wait())

		{
			
			if (selector.isReady(listener))

			{
				mtx.lock();
				sf::TcpSocket* newClient = new sf::TcpSocket;
				if (listener.accept(*newClient) == sf::Socket::Done)

				{
					sockets.push_back(newClient);
					selector.add(*newClient);
					std::cout << "New client connected: " << sockets[sockets.size() - 1]->getRemoteAddress() << std::endl;
				}

				else

				{
					delete newClient;
				}
				mtx.unlock();
			}

			else

			{
				for (int i = 0; i < sockets.size(); i++)

				{
					if (selector.isReady(*sockets[i]))

					{
						sf::Packet recievePacket;
						mtx.lock();
						sockets[i]->receive(recievePacket);
						mtx.unlock();
						broadcast(recievePacket, i);
					}
				}
			}
			
		}
	}
}

void Program::broadcast(sf::Packet& packet, int index)

{
	mtx.lock();
	for (int i = 0; i < sockets.size(); i++)

	{
		if (index != i) sockets[i]->send(packet);
	}
	mtx.unlock();
}

void Program::client()

{
	if (clientSocket.connect("192.168.0.54", 5000) == sf::Socket::Done)

	{
		std::cout << "Connected\n";
	}

	thread = new sf::Thread(&Program::executeThread, this);
	thread->launch();

	while (!quit)

	{
		getInput();
	}

	if (thread)

	{
		
		thread->wait();
		delete thread;
	}
}

void Program::getInput()

{
	std::string s;
	std::cout << "\nEnter \"exit\" to quit or message to send: ";
	getline(std::cin, s);
	if (s == "exit") quit = 1;
	mtx.lock();
	msgSend = s;
	mtx.unlock();
}

void Program::executeThread()

{
	static std::string oldMsg;
	while (!quit)

	{
		sf::Packet packetSend;
		mtx.lock();
		packetSend << msgSend;
		mtx.unlock();
		clientSocket.send(packetSend);

		std::string msg;
		sf::Packet packetRecieve;
		clientSocket.receive(packetRecieve);

		if ((packetRecieve >> msg) && oldMsg != msg && !msg.empty())

		{
			std::cout << "\n Other: " << msg << std::endl;
			oldMsg = msg;
		}
	}
}