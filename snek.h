#pragma once
#include <SFML/Graphics.hpp>
#include <Maths.h>
#include "map.h"
#include "Point.h"

class Snek
{
public:
	Snek(const Map& map);

	void update(std::vector<Snek>& snakes, const Map& map, int index);
	void draw(sf::RenderWindow& window, std::vector<Snek>& snakes, const Map& map);
	void setRotAngle(float rad);
	void setRotSpeed(float speed);
	void resetPos(const Map& map);
	void snekRektOmeter(std::vector<Snek>& snakes, int index);
	void edges(const Map& map);
	float randNumber(float max, float min);

	int speedSnek(int ch);
	int fatSnek(int ch);
	int randomInt(int min, int max);

	int bodySize;
	bool snekRekt;
	float speed;
	float rotSpeed;

	std::vector<Point> points;
	sf::CircleShape body;

	Vec2f position;
	Vec2i snekOrigin;
private:
	
	
	sf::Clock spacer, speedSnekClock, fatSnekClock;
	
	int randDist;
	bool allowedToMakePoint;
	float rotAngle;
	float randSpacer;
	float dist;

	Vec2f prev;
	Vec2f velocity;	
};