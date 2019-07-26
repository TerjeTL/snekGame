#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <Maths.h>
#include "Point.h"

class Ghost

{
public:
	Ghost(std::string id_);
	Vec2f position;
	Vec2f serverPosition;
	Vec2f velocity;
	std::vector<Point> points;
	std::string id;
	float bodySize = 3.0;

	void reset();
	void draw(sf::RenderWindow& window);
	void update();
	void correctPos();

private:
};