#pragma once
#include <SFML/Graphics.hpp>
#include <Maths.h>
#include "map.h"

class Snek
{
public:
	Snek(const Map& map);
	void update(std::vector<Snek>& snakes, Snek& snek, const Map& map);
	void draw(sf::RenderWindow& window, std::vector<Snek>& snakes, const Map& map);
	void setRotAngle(float rad);
	void setRotSpeed(float speed);
    Vec2f position;
    std::vector<Vec2f> points;
	bool snekRekt;
	float randNumber(float max, float min);
	int randomInt(int min, int max);
	sf::CircleShape body;
	int bodySize;
	float speed;
	void resetPos(const Map& map);
	float rotSpeed;
	Vec2i snekOrigin;
	int speedSnek(int ch);
	int fatSnek(int ch);
private:
	float randSpacer;
	int randDist;
	sf::Clock spacer, speedSnekClock, fatSnekClock;
	Vec2f velocity;
	float rotAngle;
	void snekRektOmeter(std::vector<Snek>& snakes);
	float dist;
	Vec2f prev;
	bool allowedToMakePoint;
};