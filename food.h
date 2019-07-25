#include <SFML/Graphics.hpp>
#include <Maths.h>
#include "map.h"
#include "snek.h"
#include "program.h"

class Food
{
public:
	Food(int id, Vec2f pos, float size, sf::Color color) : ident(id), activator(size, 12), activatorPoint(pos)
	{
		activator.setFillColor(color);
		activator.setOrigin(size, size);
		sf::Vector2f vec;
		vec.x = pos.x;
		vec.y = pos.y;
		activator.setPosition(vec);
	}
	sf::CircleShape activator;
	Vec2f activatorPoint;
	int ident;
};