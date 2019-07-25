#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "snek.h"
#include "map.h"
#include "food.h"
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
	bool running;
	Map area;

private:
	//void startGame(), addSnek();
	std::vector<Snek> snakes;
	int w, h;
	int activatorSize;
	int colors[4][3] = { {0, 255, 0},  {255, 0, 0}, {255, 0, 255}, {0, 255, 255} };
	sf::RenderWindow window;
	sf::Clock spawnClock;
	std::vector<Food> foodVec;
	std::vector<Point> foods;
};

