#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "snek.h"
#include "map.h"
#include "food.h"
#include "Ghost.h"
#include "QuadTree.h"
#include "NetworkHandler.h"

class NetworkHandler;
class Snek;

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
	void spawnFood();
	void getName();
	void reset();
	void resetAll();
	bool running;
	Map area;

	std::shared_ptr<Snek> snek;
	std::shared_ptr<NetworkHandler> networkHandler;

private:
	//void startGame(), addSnek();
	
	std::vector<Ghost> ghosts;
	int w, h;
	int quit = 0;
	int activatorSize;
	std::vector<std::vector<int>> colors = { {0, 255, 0},  {255, 0, 0}, {255, 0, 255}, {0, 255, 255}, {255, 160, 60}, {130, 60, 200}, {255, 255, 0}, {0, 0, 255}, {100, 100, 100}, {0, 255, 155} };
	sf::RenderWindow window;
	sf::Clock spawnClock;
	sf::Clock respawnClock;
	std::vector<Point> foods;
	sf::Mutex mtx;
	QuadTree* qtree;
	Program* me;
};

