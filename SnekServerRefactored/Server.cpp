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
	std::cout << "Server running..." << std::endl << std::endl;
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
					std::cout << clients.size() << " client(s) connected" << std::endl;

					if (running) inactive++;

					sf::Packet sendPacket;
					std::vector<int> color;
					chooseColor(color);
					myIDPacket myID(color[0], color[1], color[2]);
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
								std::cout << "Client " << id << " on " << ip << ":" << port << " connected on UDP" << std::endl << std::endl;
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
						
						

						if (clients[i].socket->receive(recievePacket) == sf::Socket::Disconnected)

						{
							//Do disconnect code;
							DisconnectPacket disconnectPacket;
							sf::Packet sendPacket;
							sendPacket << disconnectPacket << clients[i].id;
							broadcastTCP(sendPacket, i);
							std::cout << clients[i].id << " disconnected. " << clients.size() - 1 << " client(s) remaining" << std::endl << std::endl;
							serverReset();
							selector.remove(*clients[i].socket);
							if (i < usedColors.size()) usedColors.erase(usedColors.begin() + i);
							clients.erase(clients.begin() + i);
							i--;
							if (clients.size() == 0) foods.clear(), usedColors.clear();
						}

						else

						{
							unsigned char msg;
							sf::Packet copy = recievePacket;
							copy >> msg;
							if (msg == EATP)

							{
								EatPacket eatPacket;
								copy >> eatPacket;
								//std::cout << "Food " << eatPacket.index << " eaten" << std::endl;
								if (eatPacket.index < foods.size()) foods.erase(foods.begin() + eatPacket.index);
								updateFood();
							}

							else if (msg == RDYP)

							{
								recievePacket << clients[i].id;
								ready++;
								std::cout << clients[i].id << " readied up! " << ready << " out of " << clients.size() << std::endl << running << std::endl;
								broadcastTCP(recievePacket, i);
								
							}

							else if (msg == MCLR)

							{
								if (clients.size() > 1)

								{
									reset++;
									recievePacket << clients[i].id;
									broadcastTCP(recievePacket, i);
									std::cout << reset << " dead" << std::endl;

									if ((reset + 1) >= (clients.size() - inactive))

									{
										shouldReset = 1;
										//resetClock.restart();
									}
								}
							}

							else

							{
								recievePacket << clients[i].id;
								broadcastTCP(recievePacket, i);
							}
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
				createFood();
				if (clients.size() == 0) foods.clear();
			}

			sendDroppedPackets();

			if (ready >= (clients.size() - inactive) && !running && clients.size() != 0)

			{
				StartPacket startPacket;
				sf::Packet sendPacket;
				sendPacket << startPacket;
				broadcastTCP(sendPacket);
				running = 1;
			}

			if (shouldReset)

			{
				std::cout << "We have a winner! Reseting..." << std::endl;
				serverReset();
			}
		}
	}

	return 0;
}

void Server::sendDroppedPackets()

{
	for (int i = 0; i < clients.size(); i++)

	{
		if (clients[i].droppedPackets.size() > 0)

		{
			DroppedPacket packet = clients[i].droppedPackets.front();
			clients[i].droppedPackets.pop_front();
			broadcastTCP(packet.packet, packet.index, packet.me);
		}
	}
}

void Server::serverReset()

{
	shouldReset = 0;
	reset = 0;
	ready = 0;
	running = 0;
	inactive = 0;
	ResetPacket resetPacket;
	sf::Packet sendPacket;
	sendPacket << resetPacket;
	broadcastTCP(sendPacket);
	foods.clear();
	updateFood();
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
		if (index != i && !me) 

		{
			sf::Socket::Status status = clients[i].socket->send(packet);
			if (status != sf::Socket::Done && status != sf::Socket::Disconnected)

			{
				std::cout << "Dropped packet!" << std::endl;
				clients[i].droppedPackets.push_back(DroppedPacket(index, me,packet));
			}
		}
		else if (me && i == index)

		{
			sf::Socket::Status status = clients[i].socket->send(packet);
			if (status != sf::Socket::Done && status != sf::Socket::Disconnected)

			{
				std::cout << "Dropped packet!" << std::endl;
				clients[i].droppedPackets.push_back(DroppedPacket(index, me, packet));
			}
		}
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

void Server::createFood()

{
	if (foodClock.getElapsedTime().asSeconds() > 10.0 && clients.size() > 0 && running)

	{
		int positionX = randomInt(50, 650);
		int positionY = randomInt(50, 650);
		foods.push_back(Point(Vec2f(positionX, positionY), randomInt(1, 10), 6));
		foodClock.restart();
		updateFood();
	}
}

void Server::updateFood()

{
	sf::Packet packet;
	PointPacket pointPacket(0, 0, -1, 1);
	packet << pointPacket;
	broadcastTCP(packet);

	for (int i = 0; i < foods.size(); i++)

	{
		pointPacket = PointPacket(foods[i].position.x, foods[i].position.y, foods[i].type, foods[i].radius);
		packet.clear();
		packet << pointPacket;
		broadcastTCP(packet);
	}
}

void Server::chooseColor(std::vector<int>& color)

{
	int index = randomInt(0, colors.size() - 1);
	int used = 1;
	while (used)

	{
		int val = 0;
		for (int i = 0; i < usedColors.size() && usedColors.size() < colors.size(); i++)

		{
			if (index == usedColors[i])

			{
				val = 1;
			}
		}

		if (val) used = 1, index = randomInt(0, colors.size() - 1);
		else used = 0;
	}
	usedColors.push_back(index);
	color = colors[index];
}

int Server::randomInt(int min, int max)

{
	return rand() % (max - min + 1) + min;
}