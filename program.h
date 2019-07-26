#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "snek.h"
#include "map.h"
#include "food.h"
#include "Ghost.h"
class Program
{
public:
	Program(int width, int height);
	int mainLoop();
	int randomInt(int min, int max);
	void draw();
	void update();
	void eventHandler(sf::Event events);
	void spawnFood();
	void reset();
	bool running;
	Map area;

private:
	//void startGame(), addSnek();
	Snek snek;
	std::vector<Ghost> ghosts;
	int w, h;
	int activatorSize;
	int colors[5][3] = { {0, 255, 0},  {255, 0, 0}, {255, 0, 255}, {0, 255, 255}, {255, 160, 60} };
	sf::RenderWindow window;
	sf::Clock spawnClock;
	std::vector<Point> foods;
	sf::Mutex mtx;
	NetworkHandler networkHandler;
};

