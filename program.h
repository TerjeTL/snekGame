#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "snek.h"
#include "map.h"
#include "food.h"
#include "Ghost.h"
#include "QuadTree.h"
class Program
{
public:
	Program(int width, int height);
	~Program();
	int mainLoop();
	int randomInt(int min, int max);
	int spawnTimer;
	void draw();
	void update();
	void eventHandler(sf::Event events);
	void spawnFood(Snek snek);
	void reset();
	bool running;
	Map area;

private:
	//void startGame(), addSnek();
	Snek snek;
	std::vector<Ghost> ghosts;
	int w, h;
	int quit = 0;
	int activatorSize;
	std::vector<std::vector<int>> colors = { {0, 255, 0},  {255, 0, 0}, {255, 0, 255}, {0, 255, 255}, {255, 160, 60}, {130, 60, 200}, {255, 255, 0}, {0, 0, 255}, {100, 100, 100} };
	sf::RenderWindow window;
	sf::Clock spawnClock;
	std::vector<Point> foods;
	sf::Mutex mtx;
	QuadTree* qtree;
	
	NetworkHandler networkHandler;
};

