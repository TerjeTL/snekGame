#include "Server.h"

Server::Server()

{

}

int Server::mainLoop()

{
	listener.listen(5000);
	udpSocket.bind(5000);
	selector.add(listener);
	selector.add(udpSocket);
	while (!quit)

	{
		if (selector.wait())

		{	//Check TCP listener
			if (selector.isReady(listener))

			{
				clients.push_back(Client());
				unsigned int last = clients.size() - 1;
				clients[last].socket = std::shared_ptr<sf::TcpSocket>(new sf::TcpSocket());
				if (listener.accept(*clients[clients.size() - 1].socket) == sf::Socket::Done)

				{
					std::string id;
					generateID(id);
					
					selector.add(*clients[last].socket);

					//Setup Client
					clients[last].tcpSetup(id);
					//Console
					std::cout << "New TCP client on " << clients[last].ip << ":" << clients[last].tcpPort << " id: " << clients[last].id << std::endl;
					std::cout << clients.size() << " clients connected" << std::endl;

					sf::Packet sendPacket;
					myIDPacket myID;
					sendPacket << myID << id;
					clients[last].socket->send(sendPacket);
				}

				else

				{
					clients.pop_back();
				}
			}
			//Check UDP Socket
			else if (selector.isReady(udpSocket))

			{
				sf::Packet recievePacket;
				sf::IpAddress ip;
				unsigned short port;

				if (udpSocket.receive(recievePacket, ip, port) == sf::Socket::Done)

				{
					unsigned char msg;
					sf::Packet copy = recievePacket;
					copy >> msg;
					int connected = getUDPClient(ip, port);
					

					if (msg == UDPJ && connected == -1)

					{
						//Add UDP connection to client list
						unsigned char code;
						std::string id;
						recievePacket >> code >> id;
						for (int i = 0; i < clients.size(); i++)

						{
							if (clients[i].id == id)

							{
								clients[i].udpPort = port;
								std::cout << "Client " << id << " on " << ip << ":" << port << " connected on UDP" << std::endl;
								break;
							}
						}
					}

					else

					{
						recievePacket << clients[connected].id;
						broadcastUDP(recievePacket, connected);
					}
				}

				/*if (alive.getElapsedTime().asSeconds() > 1.0)

				{

					AlivePacket alivePacket;
					sf::Packet sendPacket;
					sendPacket << alivePacket;
					broadcastUDP(sendPacket);
					alive.restart();

				}*/
				
			}
			//Check TCP Sockets
			else

			{
				for (int i = 0; i < clients.size(); i++)

				{
					if (selector.isReady(*clients[i].socket))

					{
						sf::Packet recievePacket;
						sf::Packet copy = recievePacket;
						unsigned char msg;
						copy >> msg;

						if (clients[i].socket->receive(recievePacket) == sf::Socket::Disconnected)

						{
							//Do disconnect code;
							DisconnectPacket disconnectPacket;
							sf::Packet sendPacket;
							sendPacket << disconnectPacket << clients[i].id;
							broadcastTCP(sendPacket, i);
							std::cout << clients[i].id << " disconnected. " << clients.size() - 1 << " clients remaining" << std::endl;
							selector.remove(*clients[i].socket);
							clients.erase(clients.begin() + i);
							i--;
						}

						else

						{
							recievePacket << clients[i].id;
							broadcastTCP(recievePacket, i);
						}
					}
				}
			}

			if (ping.getElapsedTime().asSeconds() > 0.5)

			{
				AlivePacket alivePacket;
				sf::Packet sendPacket;
				sendPacket << alivePacket;
				broadcastTCP(sendPacket);
				ping.restart();
			}
		}
	}

	return 0;
}

int Server::getUDPClient(const sf::IpAddress& ip, const unsigned short& port)

{
	for (int i = 0; i < clients.size(); i++)

	{
		if (clients[i].ip == ip && clients[i].udpPort == port)

		{
			return i;
		}
	}

	return -1;
}

void Server::broadcastTCP(sf::Packet packet, int index, int me)

{
	for (int i = 0; i < clients.size(); i++)

	{
		if (index != i && !me) clients[i].socket->send(packet);
		else if (me && i == index) clients[i].socket->send(packet);
	}
}

void Server::broadcastUDP(sf::Packet packet, int index, int me)

{
	for (int i = 0; i < clients.size(); i++)

	{
		if (i != index && !me) udpSocket.send(packet, clients[i].ip, clients[i].udpPort);
		else if (me && i == index) udpSocket.send(packet, clients[i].ip, clients[i].udpPort);
	}
}

void Server::createID(std::string& s)

{
	s = "";
	static const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	for (int i = 0; i < 5; i++)

	{
		s += alphanum[rand() % (sizeof(alphanum) - 1)];
	}
}

int Server::checkID(std::string& s)

{
	for (int i = 0; i < ids.size(); i++)

	{
		if (ids[i] == s)

		{
			return 1;
		}
	}
	return 0;
}

void Server::generateID(std::string& s)

{
	std::string id;
	createID(s);
	unsigned int exists = checkID(s);
	while (exists)

	{
		createID(s);
		exists = checkID(s);
	}
}