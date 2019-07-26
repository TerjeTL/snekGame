#include "Ghost.h"

Ghost::Ghost(std::string id_): id(id_)

{

}

void Ghost::reset()

{
	points.clear();
}

void Ghost::draw(sf::RenderWindow& window)

{
	sf::CircleShape circle;
	

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