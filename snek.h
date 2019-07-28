#pragma once
#include <SFML/Graphics.hpp>
#include <Maths.h>
#include "map.h"
#include "Point.h"
#include "Ghost.h"
#include "NetworkHandler.h"
#include "QuadTree.h"

#define PI 3.14159

class Snek
{
public:
	Snek(Map& map, NetworkHandler& networkHandler_, sf::Mutex& mtx_, QuadTree*& qtree_);

	void update(std::vector<Ghost>& ghosts, Map& map, std::vector<Point>& foods);
	void posUpdate();
	void snekBodyUpdate();
	void checkFood(Point point, Map& map, std::vector<Point>& foods);
	void draw(sf::RenderWindow& window);
	void setRotAngle(float rad);
	void setRotSpeed(float speed);
	void resetPos(const Map& map);
	void snekRektOmeter(std::vector<Ghost>& ghosts, std::vector<Point>& foods, Map& map);
	void edges(Map& map);
	void setColor();
	float randNumber(float max, float min);

	int speedSnek(int ch);
	int fatSnek(int ch);
	bool squareSnek, wooshSnek, revSnek, mitosis, noHoles;
	int randomInt(int min, int max);

	int bodySize;
	bool snekRekt;
	float speed;
	float rotSpeed;

	std::vector<Point> points;
	sf::CircleShape body;
	sf::Clock squareSnekTimer, wooshTimer, revSnekTimer, mitosisClock, noHolesClock;
	
	Vec2f velocity;	
	Vec2f position;
	Vec2i snekOrigin;
private:
	
	sf::Mutex& mtx;
	sf::Clock spacer, speedSnekClock, fatSnekClock, updateClock, sendPosClock, sendPointClock;
	sf::Color color;
	sf::VertexArray drawPoints;
	int randDist, pointsAllowed;
	bool allowedToMakePoint;
	float rotAngle;
	float randSpacer;
	float dist;
	NetworkHandler& networkHandler;
	Vec2f prev;
	QuadTree*& qtree;
	
};