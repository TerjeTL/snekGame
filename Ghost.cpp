#include "Ghost.h"

Ghost::Ghost(std::string id_, unsigned char r, unsigned char g, unsigned char b): id(id_), velocity(0, 1)

{
	color.r = r;
	color.g = g;
	color.b = b;
}

void Ghost::reset()

{
	points.clear();
}

void Ghost::update()

{
	position += velocity;
	//serverPosition += velocity;
	//position = serverPosition;
}

void Ghost::correctPos()

{
	position = serverPosition;
}

void Ghost::draw(sf::RenderWindow& window)

{
	sf::CircleShape circle;
	circle.setFillColor(color);

	for (int i = 0; i < points.size(); i++)

	{
		circle.setRadius(points[i].radius);
		circle.setOrigin(circle.getGlobalBounds().width / 2, circle.getGlobalBounds().height / 2);
		circle.setPosition(points[i].position.x, points[i].position.y);
		window.draw(circle);
	}

	circle.setRadius(bodySize);
	circle.setOrigin(circle.getGlobalBounds().width / 2, circle.getGlobalBounds().height / 2);
	circle.setPosition(position.x, position.y);
	window.draw(circle);
}