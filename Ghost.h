#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <Maths.h>
#include "Point.h"

class Ghost

{
public:
	Ghost();
	Vec2f position;
	std::vector<Point> points;
	std::string id;
	float bodySize = 3.0;

	void reset();
	void draw(sf::RenderWindow& window);

private:

};