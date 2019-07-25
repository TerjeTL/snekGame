#include "snek.h"
#include <Maths.h>
#include <math.h>

Snek::Snek(const Map& map) : bodySize(3), body(3, 70), rotAngle(0.0), dist(0), snekOrigin(map.origin)
{
	snekRekt, allowedToMakePoint = false, true;
	body.setOrigin(sf::Vector2f(3, 3));
	points.push_back(Point(position, SNAKE, 3));
	randSpacer = 0.6;
	randDist = 20;
	resetPos(map);
	rotSpeed = 0.08;
	speed = 2.0f;
}

void Snek::update(std::vector<Snek>& snakes, const Map& map, int index, std::vector<Point>& foods)
{
	body.setRadius(bodySize);
	velocity.rotateInPlaze(rotAngle);
	velocity = normalize(velocity)*speed;
	position += velocity;
	
	if (spacer.getElapsedTime().asSeconds() > randSpacer)
	{
		dist = 0;
		spacer.restart();
		randSpacer = randNumber(1.6, 0.6);
		//std::cout << randSpacer << std::endl;
		randDist = randomInt(25, 7);

	}
	if (dist < randDist)
	{
		spacer.restart();
	}
	else
	{
		points.push_back(Point(position, SNAKE, bodySize));
	}
	dist += distance(prev, position);
	//std::cout << snek.dist << std::endl;
	prev = position;

	checkFood(foods);
	edges(map);

	snekRektOmeter(snakes, index);

	speedSnek(0);

	int t2(fatSnek(0));
	if (t2 == -2) fatSnek(-1);
	else if (t2 == 2) fatSnek(1);
	else;

	if (snekRekt) {
		std::cout << "snek ded" << std::endl;
	}
}

void Snek::checkFood(std::vector<Point>& foods)

{
	for (int j = 0; j < points.size(); j++)
	{
		for (int k = 0; k < foods.size(); k++)
		{
			if (distanceSquared(points[j].position, foods[k].position) < pow(bodySize + foods[k].radius, 2))
			{
				switch (foods[k].type)
				{
				case FAST:
					speedSnek(1);
					std::cout << "speed" << std::endl;
					break;

				case SLOW:
					speedSnek(-1);
					break;

				case FAT: //fat snek
					fatSnek(1);
					break;

				case THIN: //skinny snek
					fatSnek(-1);
					break;
				}
				foods.erase(foods.begin() + k);
			}
		}
	}
}

void Snek::edges(const Map& map)

{
	if (position.x > (snekOrigin.x + map.size)) position.x = snekOrigin.x;
	else if (position.x < snekOrigin.x) position.x = snekOrigin.x + map.size;
	//std::cout << map.size << std::endl;
	if (position.y > (snekOrigin.y + map.size)) position.y = snekOrigin.y;
	else if (position.y < snekOrigin.y) position.y = (snekOrigin.y + map.size);
}

void Snek::draw(sf::RenderWindow& window, std::vector<Snek>& snakes, const Map& map)
{

	for (int i = 0; i < points.size(); i++)
	{
		body.setRadius(points[i].radius);
		body.setOrigin(body.getGlobalBounds().width/2, body.getGlobalBounds().height/2);
		body.setPosition(points[i].position.x, points[i].position.y);
		window.draw(body);
	}
	body.setPosition({ position.x, position.y });
	window.draw(body);
	
}

void Snek::resetPos(const Map& map)
{
	position.x = randomInt(snekOrigin.x+50, (snekOrigin.x + map.size)-50);
	position.y = randomInt(snekOrigin.y+50, (snekOrigin.y + map.size)-50);
	int a = randomInt(-10, 10);
	int b = randomInt(-10, 10);
	float c = sqrtf(powf((float)a, 2.0) + powf((float)b, 2.0));
	velocity.x = a/c;
	velocity.y = b/c;
	velocity = velocity * speed;
}

void Snek::setRotAngle(float rad)
{
	rotAngle = rad;
}

void Snek::setRotSpeed(float speed)
{
	rotSpeed = speed;
}

void Snek::snekRektOmeter(std::vector<Snek>& snakes, int index)
{
	//checks if he reks himself
	for (int i = 0; i < snakes.size(); i++)

	{
		int end = 0;
		int start = 1;
		if (i == index)

		{
			if (points.size() < 20) start = 0;
			end = 20;
		}

		for (int j = 0; j < snakes[i].points.size() - end && start; j++)

		{
			Point point = snakes[i].points[j];
			if (distanceSquared(position, point.position) < pow(bodySize + point.radius, 2))
			{
				snekRekt = true;
				break;
			}
		}
	}

	/*
	for (int i = 0; i < snakes.size(); i++)  // B E A U T I F U L FOR LOOP
	{
		if (snakes[i].points.size() > 30)
		{
			for (int j = 0; j < snakes[i].points.size()-30; j++)
			{
				if (distance(snakes[i].position, snakes[i].points[j]) < 5.5f)
				{
					snakes[i].snekRekt = true;
					break;
				}
			}
		}
		//checks for danger noodles
		for (int h = 0; h < snakes.size(); h++)
		{
			if (i != h)
			{
				for (int k = 0; k < snakes[h].points.size(); k++)
				{
					if (distanceSquared(snakes[i].position, snakes[h].points[k]) < 30.25f)
					{
						snakes[i].snekRekt = true;
						break;
					}
				}
			}
		}
	}
	*/
}

void Snek::speedSnek(int ch)
{
	
	if (ch == 1)
	{
		speed += 2.0f;
		speedSnekClock.restart();
	}
	else if (ch == -1 && speed > 1.4f)
	{
		speed -= 1.0f;
		speedSnekClock.restart();
	}
	else if (ch == -1 && speed < 1.01f)
	{
		speed = 0.4f;
		speedSnekClock.restart();
	}
	else
	{
		if (speedSnekClock.getElapsedTime().asSeconds() >= 3 && ch ==0)
		{
			if (speed < 1.9f) return 2;
			else if (speed > 2.1f) return -2;
			else return 0;
		}
	}
}

int Snek::fatSnek(int ch)
{
	if (ch == 1)
	{
		bodySize += 2;
		std::cout << "here" << std::endl;
		fatSnekClock.restart();
		return 0;
	}
	else if (ch == -1 && bodySize > 1.4)
	{
		bodySize -= 1;
		fatSnekClock.restart();
		return 0;
	}
	else
	{
		if (fatSnekClock.getElapsedTime().asSeconds() >= 3)
		{
			if (bodySize < 2.9 && bodySize > 1.9)
			{
				bodySize = 3;
				return 0;
			}
			else if (bodySize > 3.1 && bodySize < 4.9)
			{
				bodySize = 3;
				return 0;
			}
			else if (bodySize < 2.9) return 2;
			else if (bodySize> 3.1) return -2;
			else return 0;
		}
	}
}

float Snek::randNumber(float max, float min)
{
	float v = 100.0;
	int minI = min * v;
	int maxI = max * v;
	int randInt = randomInt(minI, maxI);
	return (float)randInt / v;
}

int Snek::randomInt(int min, int max)

{
	return rand() % (max - min + 1) + min;
}