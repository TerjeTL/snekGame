#include <Maths.h>
#include <math.h>
#include "functions.h"

Functions::Functions()
{
	spawnClock.restart();
	activatorSize = 6;
}

void Functions::update(const Map& maps, std::vector<Snek>& snakes)
{

	collision(snakes);
	for (int i = 0; i < foodVec.size(); i++)
	{
		std::cout << i << std::endl;
	}

	if (spawnClock.getElapsedTime().asSeconds() > 3)
	{
		Vec2f position;
		position.x = randomInt(maps.origin.x, maps.origin.x + maps.size);
		position.y = randomInt(maps.origin.y, maps.origin.y + maps.size);

		int rand = randomInt(0, 3);
		
		if (rand == 0)
		{	//fass boii
			Food speedyBoi(0, position, activatorSize, sf::Color::Green);
			foodVec.push_back(speedyBoi);
		}
		else if (rand == 1)
		{ //slow boii
			Food slowBoi(1, position, activatorSize, sf::Color::Red);
			foodVec.push_back(slowBoi);
		}
		else if (rand == 2) //fat snek
		{
			Food fatBoi(2, position, activatorSize, sf::Color::Magenta);
			foodVec.push_back(fatBoi);
		}
		else if (rand == 3)
		{	//skinny snek
			Food skinnyBoi(3, position, activatorSize, sf::Color::Cyan);
			foodVec.push_back(skinnyBoi);
		}
		

		spawnClock.restart();
	}

}

void Functions::collision(std::vector<Snek>& snakes)
{
	for (int i = 0; i < snakes.size(); i++)
	{
		for (int j = 0; j < snakes[i].points.size(); j++)
		{
			for (int k = 0; k < foodVec.size(); k++)
			{
				if (distance(snakes[i].points[j], foodVec[k].activatorPoint) < snakes[i].bodySize + activatorSize)
				{
					switch (foodVec[k].ident)
					{
					case 0:
						snakes[i].speedSnek(1);
						std::cout << "speed" << std::endl;
						break;

					case 1:
						snakes[i].speedSnek(-1);
						break;

					case 2: //fat snek
						snakes[i].fatSnek(1);
						break;

					case 3: //skinny snek
						snakes[i].fatSnek(-1);
						break;
					}
					foodVec.erase(foodVec.begin() + k);
				}
			}
		}
	}
}

void Functions::draw(sf::RenderWindow& window)
{
	for (int i = 0; i < foodVec.size(); i++)
	{
		window.draw(foodVec[i].activator);
	}
}


int Functions::randomInt(int min, int max)
{
	return rand() % (max - min + 1) + min;
}