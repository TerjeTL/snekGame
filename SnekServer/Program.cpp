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
	
	//listener.listen(5000);
	//selector.add(listener);
	/*
	while (!quit)

	{
		if (selector.wait())

		{
			if (selector.isReady(listener))

			{
				sf::UdpSocket* newClient = new sf::UdpSocket;
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
							DisconnectPacket packet;
							sf::Packet sendPacket;
							sendPacket << packet;
							sendPacket << sockets[i]->id;
							broadcast(sendPacket, i);

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
							unsigned char msg;
							copy >> msg;
							//std::cout << msg << std::endl;

							if (msg == CREA)

							{
								std::cout << "create" << std::endl;
							}

							if (msg == MCLR)

							{
								std::cout << id << " has reset!" << std::endl;
							}
							broadcast(recievePacket, i);
						}
						
					}
				}
			}
			
		}

		if (alive.getElapsedTime().asSeconds() > 1.0)

		{
			AlivePacket packet;
			sf::Packet sendPacket;
			sendPacket << packet;
			broadcast(sendPacket);
			alive.restart();
		}
	}
	*/
	serverSocket.bind(5000);
	serverSocket.setBlocking(true);
	selector.add(serverSocket);
	while (!quit)

	{
		if (selector.wait(sf::seconds(2)))

		{
			
			if (selector.isReady(serverSocket))

			{
				sf::Packet packet;
				sf::IpAddress ip;
				unsigned short port;

				if (serverSocket.receive(packet, ip, port) == sf::Socket::Done)

				{
					int connected = -1;
					for (int i = 0; i < clients.size(); i++)

					{
						if (clients[i].ip == ip && clients[i].port == port)

						{
							connected = i;
							break;
						}
					}

					if (connected == -1)

					{
						std::string s;
						generateID(s);
						if (!checkID(s))

						{
							while (!checkID(s))

							{
								generateID(s);
							}
						}
						clients.push_back(UdpClient(ip, port, s));
						sf::Packet sendPacket;
						myIDPacket myID;
						sendPacket << myID << s;
						broadcastUDP(sendPacket, clients.size() - 1, 1);
						std::cout << "New client " << ip << ":" << port << " id " << s << std::endl;
						std::cout << clients.size() << " clients connected" << std::endl;
					}

					else

					{
						sf::Packet copy = packet;
						unsigned char msg;
						copy >> msg;
						//std::cout << msg << std::endl;

						if (msg == CREA)

						{
							//std::cout << "create" << std::endl;
						}

						if (msg == MCLR)

						{
							std::cout << clients[connected].id << " has reset!" << std::endl;
						}

						if (msg == ALIV)

						{
							clients[connected].clock.restart();
						}

						if (msg == DSCT)

						{
							disconnect(connected);
						}

						packet << clients[connected].id;
						broadcastUDP(packet, connected);
					}
				}

				if (alive.getElapsedTime().asSeconds() > 1.0)

				{

					AlivePacket alivePacket;
					sf::Packet sendPacket;
					sendPacket << alivePacket;
					broadcastUDP(sendPacket);
					alive.restart();
					
				}
			}
		}

		checkTimeout();
	}
}

void Program::disconnect(int index)

{
	DisconnectPacket disconnectPacket;
	sf::Packet sendPacket;
	sendPacket << disconnectPacket;
	sendPacket << clients[index].id;
	std::cout << clients[index].id << " on " << clients[index].ip << ":" << clients[index].port << " has timedout/disconnected" << std::endl;
	broadcastUDP(sendPacket, index);
	clients.erase(clients.begin() + index);
	std::cout << clients.size() << " remaining" << std::endl;
}

void Program::checkTimeout()

{
	for (int i = 0; i < clients.size(); i++)

	{
		if (clients[i].clock.getElapsedTime().asSeconds() > 10.0)

		{
			disconnect(i);
			i--;
		}
	}
}

void Program::broadcastUDP(sf::Packet& packet, int index, int me)

{
	for (int i = 0; i < clients.size(); i++)

	{
		if (i != index && !me) serverSocket.send(packet, clients[i].ip, clients[i].port);
		else if (me && i == index) serverSocket.send(packet, clients[i].ip, clients[i].port);
	}
}

int Program::checkID(std::string& id)

{
	for (int i = 0; i < ids.size(); i++)

	{
		if (ids[i] == id)

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