#include "Ghost.h"

Ghost::Ghost(std::string id_, unsigned char r, unsigned char g, unsigned char b): id(id_), velocity(0, 1)

{
	color.r = r;
	color.g = g;
	color.b = b;
	drawPoints.setPrimitiveType(sf::Quads);
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
	circle.setFillColor(color);

	for (int i = 0; i < points.size(); i++)

	{
		//circle.setRadius(points[i].radius);
		//circle.setOrigin(circle.getGlobalBounds().width / 2, circle.getGlobalBounds().height / 2);
		//circle.setPosition(points[i].position.x, points[i].position.y);
		//window.draw(circle);
		if (i > 0)

		{
			Vec2f pos = points[i].position;
			Vec2f prev = points[i - 1].position;
			Vec2f diff = pos - prev;

			if (diff.magnitudeInPlace() < 6.0)

			{
				float angle = atan(diff.y / diff.x);
				float r = points[i].radius;
				diff.rotateInPlaze(-angle);
				Vec2f tl = Vec2f(0, -r);
				Vec2f tr = Vec2f(diff.x, -r);
				Vec2f br = Vec2f(diff.x, r);
				Vec2f bl = Vec2f(0, r);

				tl.rotateInPlaze(angle);
				tr.rotateInPlaze(angle);
				br.rotateInPlaze(angle);
				bl.rotateInPlaze(angle);

				tl += prev;
				tr += prev;
				br += prev;
				bl += prev;


				drawPoints.append(sf::Vertex(sf::Vector2f(tl.x, tl.y), color));
				drawPoints.append(sf::Vertex(sf::Vector2f(tr.x, tr.y), color));
				drawPoints.append(sf::Vertex(sf::Vector2f(br.x, br.y), color));
				drawPoints.append(sf::Vertex(sf::Vector2f(bl.x, bl.y), color));
			}


			else

			{
				circle.setRadius(points[i].radius);
				circle.setOrigin(circle.getGlobalBounds().width / 2, circle.getGlobalBounds().height / 2);
				circle.setPosition(pos.x, pos.y);
				window.draw(circle);

				circle.setRadius(points[i].radius);
				circle.setOrigin(circle.getGlobalBounds().width / 2, circle.getGlobalBounds().height / 2);
				circle.setPosition(prev.x, prev.y);
				window.draw(circle);
			}
		}
	
		if (i == points.size() - 1 || i == 0)

		{
			circle.setRadius(points[i].radius);
			circle.setOrigin(circle.getGlobalBounds().width / 2, circle.getGlobalBounds().height / 2);
			circle.setPosition(points[i].position.x, points[i].position.y);
			window.draw(circle);
		}
	}

	window.draw(drawPoints);
	circle.setRadius(bodySize);
	circle.setOrigin(circle.getGlobalBounds().width / 2, circle.getGlobalBounds().height / 2);
	circle.setPosition(position.x, position.y);
	window.draw(circle);
}