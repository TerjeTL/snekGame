#include "snek.h"

Snek::Snek(Map& map, NetworkHandler& networkHandler_, sf::Mutex& mtx_) : bodySize(3), body(3, 70), rotAngle(0.0), dist(0), snekOrigin(map.origin), position(100, 100), velocity(1, 0),
networkHandler(networkHandler_), mtx(mtx_)
{
	resetPos(map);
	snekRekt, allowedToMakePoint = false, true;
	body.setOrigin(sf::Vector2f(3, 3));
	points.push_back(Point(position, SNAKE, 3));
	randSpacer = 0.6;
	randDist = 20;
	rotSpeed = 0.08;
	speed = 2.0f;
}

void Snek::update(std::vector<Ghost>& ghosts, Map& map, std::vector<Point>& foods)
{
	body.setRadius(bodySize);
	velocity.rotateInPlaze(rotAngle);
	velocity = normalize(velocity)*speed;
	position += velocity;

	networkHandler.sendPos(position, velocity);
	
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
		networkHandler.sendPoint(Point(position, SNAKE, bodySize));
	}
	dist += distance(prev, position);
	//std::cout << snek.dist << std::endl;
	prev = position;

	checkFood(foods, map);
	edges(map);

	snekRektOmeter(ghosts);

	int ch = speedSnek(0);
	if (ch != 0) speedSnek(ch);

	int ch2 = fatSnek(0);
	if (ch2 != 0) fatSnek(ch2);


	if (snekRekt) {
		std::cout << "snek ded" << std::endl;
	}
}

void Snek::checkFood(std::vector<Point>& foods, Map& map)

{
	for (int j = 0; j < points.size(); j++)
	{
		for (int k = 0; k < foods.size(); k++)
		{
			if (distanceSquared(points[j].position, foods[k].position) < pow(bodySize + foods[k].radius, 2))
			{
				std::cout << foods[k].type << std::endl;
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
				case BORDER: //flashy travely boarders
					map.papersPleaseDisabled(1);
					break;
				case SQUARE: //snek is playing snakes
					squareSnek = true;
					squareSnekTimer.restart();
				}
				foods.erase(foods.begin() + k);
			}
		}
	}
}

void Snek::edges(Map& map)

{
	map.papersPleaseDisabled(0);

	if (map.flashyBoarder)
	{
		if ((position.x + bodySize) > (snekOrigin.x + map.size)) position.x = snekOrigin.x + bodySize;
		else if ((position.x - bodySize) < snekOrigin.x) position.x = snekOrigin.x + map.size - bodySize;
		//std::cout << map.size << std::endl;
		if ((position.y + bodySize) > (snekOrigin.y + map.size)) position.y = snekOrigin.y + bodySize;
		else if ((position.y - bodySize) < snekOrigin.y) position.y = (snekOrigin.y + map.size - bodySize);
	}
	else
	{
		if (((position.x + bodySize) > (snekOrigin.x + map.size)) || ((position.x - bodySize) < snekOrigin.x) || ((position.y + bodySize) > (snekOrigin.y + map.size)) || ((position.y - bodySize) < snekOrigin.y)) snekRekt = true;
	}
}

void Snek::draw(sf::RenderWindow& window)
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
	float angle = (float)randomInt(0, 359);
	velocity.rotateInPlaze(PI*angle / 180.0);
}

void Snek::setRotAngle(float rad)
{
	if (squareSnek) rotAngle = rad;
	else 
	{
		rotAngle = rad * rotSpeed;
	}
	
}

void Snek::setRotSpeed(float speed)
{
	rotSpeed = speed;
}

void Snek::snekRektOmeter(std::vector<Ghost>& ghosts)
{
	//checks if he reks himself
	for (int i = 0; i < ghosts.size(); i++)

	{
		for (int j = 0; j < ghosts[i].points.size(); j++)

		{
			Point point = ghosts[i].points[j];
			if (distanceSquared(position, point.position) < pow(bodySize + point.radius, 2))
			{
				snekRekt = true;
				break;
			}
		}
	}

	int start = 1;
	if (points.size() < 20) start = 0;

	for (int i = 0; i < points.size() - 20 && start; i++)

	{
		Point point = points[i];
		if (distanceSquared(position, point.position) < pow(bodySize + point.radius, 2))
		{
			snekRekt = true;
			break;
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

int Snek::speedSnek(int ch)
{
	
	if (ch == 1)
	{
		speed += 1.0f;
		speedSnekClock.restart();
		return 0;
	}

	else if (ch == -1 && speed > 1.5f)
	{
		speed -= 1.0f;
		speedSnekClock.restart();
		return 0;
	}
	else if (ch == -1 && speed < 1.1f)
	{
		speed = 0.8f;
		speedSnekClock.restart();
		return 0;
	}
	else
	{
		if (ch == 0)
		{
			if (speed < 2.9f && speed > 1.1)
			{
				speed = 2.0f;
				return 0;
			}

			else if (speedSnekClock.getElapsedTime().asSeconds() > 3)
			{
				if (speed < 2.0f) return 1;
				else if (speed > 2.0f) return -1;
			}
			else return 0;
		}
	}
}

int Snek::fatSnek(int ch)
{
	if (ch == 1)
	{
		bodySize += 2;
		fatSnekClock.restart();
		return 0;
	}
	else if (ch == -1 && bodySize >= 3)
	{
		bodySize -= 2;
		fatSnekClock.restart();
		return 0;
	}
	else if (ch == -1 && bodySize < 3)
	{
		bodySize = 1;
		fatSnekClock.restart();
		return 0;
	}
	else
	{
		if (ch == 0)
		{
			if (bodySize < 4.9 && bodySize > 1.1)
			{
				bodySize = 3;
				return 0;
			}
			else if ((fatSnekClock.getElapsedTime().asSeconds() > 3))
			{
			if (bodySize < 1.1) return 1;
			else if (bodySize > 4) return -1;
			}
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