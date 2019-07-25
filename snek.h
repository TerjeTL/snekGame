#pragma once
#include <SFML/Graphics.hpp>
#include <Maths.h>
#include "map.h"
#include "Point.h"
#include "Ghost.h"

#define PI 3.14159

class Snek
{
public:
	Snek(Map& map);

	void update(std::vector<Ghost>& ghosts, Map& map, std::vector<Point>& foods);
	void checkFood(std::vector<Point>& foods, Map& map);
	void draw(sf::RenderWindow& window);
	void setRotAngle(float rad);
	void setRotSpeed(float speed);
	void resetPos(const Map& map);
	void snekRektOmeter(std::vector<Ghost>& ghosts);
	void edges(Map& map);
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