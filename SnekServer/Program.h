#pragma once
#include <iostream>
#include <SFML/Network.hpp>

class Program

{
public:

	Program();
	int mainLoop();

private:
	sf::TcpListener listener;
};