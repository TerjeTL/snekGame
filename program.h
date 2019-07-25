#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "snek.h"
#include "map.h"

class Program
{
public:
	Program(int width, int height);
	int mainLoop();
	void draw();
	void eventHandler(sf::Event events);
	bool running;
	Map area;
private:
	//void startGame(), addSnek();
	std::vector<Snek> snakes;
	int w, h;
	sf::RenderWindow window;
};

