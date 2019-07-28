#pragma once
#include <SFML/Graphics.hpp>
#include <random>
#include <Maths.h>
#include <ctime>
#include "map.h"
#include "Point.h"
#include "Ghost.h"
//#include "NetworkHandler.h"
#include "QuadTree.h"
#include "program.h"


#define PI 3.14159

class Program;

class Snek
{
public:
	Snek(Map& map, sf::Mutex& mtx_, QuadTree*& qtree_, Program*& program_);

	void update(std::vector<Ghost>& ghosts, Map& map, std::vector<Point>& foods);
	void posUpdate();
	void snekBodyUpdate();
	void checkFood(Point point, Map& map, std::vector<Point>& foods);
	void draw(sf::RenderWindow& window, Map& map);
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
	std::string name = "unamed";
	sf::CircleShape body;
	sf::Clock squareSnekTimer, wooshTimer, revSnekTimer, mitosisClock, noHolesClock;
	
	Vec2f velocity;	
	Vec2f position;
	Vec2i snekOrigin;
	sf::Clock alive;
private:
	
	sf::Mutex& mtx;
	sf::Clock spacer, speedSnekClock, fatSnekClock, updateClock, sendPosClock, sendPointClock;
	sf::Color color;
	sf::VertexArray drawPoints;
	int randDist, pointsAllowed;
	bool allowedToMakePoint;
	float rotAngle;
	float randSpacer;
	float dist, distHoles;
	float pointSpacing = 5;
	//std::shared_ptr<NetworkHandler>& networkHandler;
	Vec2f prev;
	QuadTree*& qtree;
	std::mt19937 generator;
	Program*& program;
};