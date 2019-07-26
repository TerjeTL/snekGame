#include "Program.h"

Program::Program()

{

}

Program::~Program()

{
	for (int i = 0; i < sockets.size(); i++)

	{
		delete sockets[i];
	}

	sockets.clear();
}

int Program::mainLoop()

{
	

	char who;
	std::cout << "Server (s) or client (c)? ";
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
	
	listener.listen(5000);
	selector.add(listener);
	
	while (!quit)

	{
		if (selector.wait())

		{
			if (selector.isReady(listener))

			{
				sf::TcpSocket* newClient = new sf::TcpSocket;
				Socket* socket = new Socket(newClient);
				if (listener.accept(*newClient) == sf::Socket::Done)

				{
					sockets.push_back(socket);
					std::string s;
					generateID(s);

					if (!checkID(s))

					{
						while (!checkID(s))

						{
							generateID(s);
						}
					}

					sockets[sockets.size() - 1]->id = s;

					selector.add(*newClient);
					//std::cout << "1" << std::endl;
					std::cout << "New client connected: " << sockets[sockets.size() - 1]->socket->getRemoteAddress() << " with id " << sockets[sockets.size() - 1]->id << std::endl;
					std::cout << sockets.size() << " clients connected!" << std::endl;

					myIDPacket idPacket;
					sf::Packet sendPacket;
					sendPacket << idPacket;
					sendPacket << sockets[sockets.size() - 1]->id;
					sockets[sockets.size() - 1]->socket->send(sendPacket);
				}

				else

				{
					delete socket;
				}
			}

			else

			{
				for (int i = 0; i < sockets.size(); i++)

				{
					//std::size_t dummy;
					//if (sockets[i]->socket->receive(&dummy, 0, dummy) == sf::Socket::Disconnected)
					//
					//{
					//	delete sockets[i];
					//	index = i;
					//}



					if (selector.isReady(*(sockets[i]->socket)))

					{
						sf::Packet recievePacket;
						if (sockets[i]->socket->receive(recievePacket) == sf::Socket::Disconnected)

						{
							std::cout << sockets[i]->id << " disconnected!" << std::endl;
							selector.remove(*sockets[i]->socket);
							delete sockets[i];
							sockets.erase(sockets.begin() + i);
							std::cout << sockets.size() << " clients connected!" << std::endl;
							i--;
							
						}

						else

						{
							std::string id = sockets[i]->id;
							recievePacket << id;
							sf::Packet copy = recievePacket;
							std::string msg;
							copy >> msg;

							if (msg == "mclr")

							{
								std::cout << id << " has reset!" << std::endl;
							}

							broadcast(recievePacket, i);
						}
						
					}
				}
			}
			
		}
	}
}

int Program::checkID(std::string& id)

{
	for (int i = 0; i < sockets.size(); i++)

	{
		if (sockets[i]->id == id)

		{
			return 0;
		}
	}

	return 1;
}

void Program::generateID(std::string& id)

{
	static const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	for (int i = 0; i < 10; i++)

	{
		id += alphanum[rand() % (sizeof(alphanum) - 1)];
	}
}

void Program::broadcast(sf::Packet& packet, int index)

{
	for (int i = 0; i < sockets.size(); i++)

	{
		if (index != i) sockets[i]->socket->send(packet);
	}
}

void Program::client()

{
	//clientSocket.setBlocking(false);
	if (clientSocket.connect("127.0.0.1", 5000) == sf::Socket::Done)

	{
		std::cout << "Connected\n";
	}

	thread = new sf::Thread(&Program::executeThread, this);
	thread->launch();

	while (!quit)

	{
		getInput();
		sf::Packet packetSend;
		packetSend << msgSend;
		if (msgSend != "") clientSocket.send(packetSend);
		msgSend = "";
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
	//clientSocket.setBlocking(false);
	while (!quit)

	{
		std::string msg;
		sf::Packet packetRecieve;
		if (clientSocket.receive(packetRecieve) != sf::Socket::NotReady)

		{
			if ((packetRecieve >> msg) && !msg.empty())

			{
				std::cout << "\n Other: " << msg << std::endl;
			}
		}
	}
}