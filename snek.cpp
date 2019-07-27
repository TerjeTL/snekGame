#include "snek.h"

Snek::Snek(Map& map, NetworkHandler& networkHandler_, sf::Mutex& mtx_, QuadTree*& qtree_) : bodySize(3), body(3, 70), rotAngle(0.0), dist(0), snekOrigin(map.origin), position(100, 100), velocity(1, 0),
networkHandler(networkHandler_), mtx(mtx_), squareSnek(false), qtree(qtree_), wooshSnek(false), revSnek(false), mitosis(false), noHoles(false)
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
	posUpdate();

	snekBodyUpdate(); //who tf knows how any of this works. see especially below

	networkHandler.sendPos(position, velocity, pointsAllowed, bodySize);

	//checkFood(foods, map);
	edges(map);

	if (!wooshSnek) snekRektOmeter(ghosts, foods, map);

	int ch = speedSnek(0);
	if (ch != 0) speedSnek(ch);

	int ch2 = fatSnek(0);
	if (ch2 != 0) fatSnek(ch2);


	if (snekRekt) {
		std::cout << "snek ded" << std::endl;
	}
}

void Snek::posUpdate()
{
	if (revSnekTimer.getElapsedTime().asSeconds() > 10) revSnek = false;
	if (revSnek) velocity.rotateInPlaze(-rotAngle);
	else velocity.rotateInPlaze(rotAngle);
	body.setRadius(bodySize);
	velocity = normalize(velocity)*speed;
	position += velocity;
}

void Snek::snekBodyUpdate()
{
	if (wooshTimer.getElapsedTime().asSeconds() > 10) wooshSnek = false;
	if (noHolesClock.getElapsedTime().asSeconds() > 10) noHoles = false;

	if (spacer.getElapsedTime().asSeconds() > randSpacer) //All that random numbers jazz
	{
		dist = 0;
		spacer.restart();
		randSpacer = randNumber(1.6, 0.6);
		randDist = randomInt(3 * bodySize + 9, 3 * bodySize + 25);
		//std::cout << randDist << std::endl;

	}

	if ((dist < randDist || wooshSnek) && !noHoles) // if/else -> points push-back.
	{
		spacer.restart();
		pointsAllowed = 0;
	}
	else
	{
		points.push_back(Point(position, SNAKE, bodySize));
		pointsAllowed = 1;
		//networkHandler.sendPoint(Point(position, SNAKE, bodySize));
	}
	dist += distance(prev, position);
	//std::cout << snek.dist << std::endl;
	prev = position;
}

void Snek::checkFood(Point point, Map& map, std::vector<Point>& foods)

{
	
	switch (point.type)

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
			break;

		case WOOSH:
			wooshSnek = true;
			wooshTimer.restart();
			break;

		case REVSNEK:
			revSnek = true;
			revSnekTimer.restart();
			break;

		case MITOSIS:
			mitosis = true;
			mitosisClock.restart();
			break;

		case NOHOLES:
			noHoles = true;
			noHolesClock.restart();
			break;
	}
	foods.erase(foods.begin() + point.id);

	for (int i = 0; i < foods.size(); i++)

	{
		foods[i].id = i;
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
	sf::CircleShape temp = body;
	temp.setPosition({ position.x, position.y }); // shitty code but works :))
	temp.setFillColor(sf::Color::White);
	window.draw(temp);
}

void Snek::resetPos(const Map& map)
{
	position.x = randomInt(snekOrigin.x+50, (snekOrigin.x + map.size)-50);
	position.y = randomInt(snekOrigin.y+50, (snekOrigin.y + map.size)-50);
	float angle = (float)randomInt(0, 359);
	velocity.rotateInPlaze(PI*angle / 180.0);
}

void Snek::setRotAngle(float rad) // what is this shit
{
	if (squareSnek)
	{
		rotAngle = rad;
	}
	else 
	{
		rotAngle = rad * rotSpeed;
	}
	
}

void Snek::setRotSpeed(float speed) // and this..
{
	rotSpeed = speed;
}

void Snek::snekRektOmeter(std::vector<Ghost>& ghosts, std::vector<Point>& foods, Map& map)
{
	//checks if he reks himself
	/*
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
	*/
	std::vector<Point> pointsFound;
	if (qtree != nullptr) qtree->query(pointsFound, Boundary(1.0, 1.0, position.x, position.y, 10));


	for (int i = 0; i < pointsFound.size(); i++)

	{
		Point point = pointsFound[i];
		if (distanceSquared(position, point.position) < pow(bodySize + point.radius, 2))
		{
			if (point.type == SNAKE)

			{
				snekRekt = true;
				break;
			}
			
			else

			{
				checkFood(point, map, foods);
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