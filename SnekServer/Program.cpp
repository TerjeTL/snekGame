#include "Program.h"

Program::Program()

{

}

int Program::mainLoop()

{
	if (listener.listen(5000) != sf::Socket::Done)

	{
		//error
		std::cout << "Error connecting" << std::endl;
	}

	sf::TcpSocket client;

	if (listener.accept(client) != sf::Socket::Done)

	{

	}
}