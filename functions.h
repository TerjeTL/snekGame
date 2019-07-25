#pragma once
#include <SFML/Graphics.hpp>
#include <Maths.h>
#include "map.h"
#include "snek.h"
#include "program.h"
#include "food.h"

class Functions
{
public:
	Functions();
	int activatorSize;
	void update(const Map& map, std::vector<Snek>& snakes);
	void draw(sf::RenderWindow& window);
	void collision(std::vector<Snek>& snakes);
	std::vector<Food> foodVec;

	sf::Clock spawnClock;
	int randomInt(int min, int max);
};