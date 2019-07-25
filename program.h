#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "functions.h"
#include "snek.h"
#include "map.h"
class Program
{
public:
	class Functions;
	Program(int width, int height);
	int mainLoop();
	void draw();
	void update();
	void eventHandler(sf::Event events);
	bool running;
	Map area;

private:
	Functions gameEvents;
	//void startGame(), addSnek();
	std::vector<Snek> snakes;
	int w, h;
	sf::RenderWindow window;
};

