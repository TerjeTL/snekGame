#pragma once
#include <Maths.h>

#define SNAKE 0
#define FAST 1
#define SLOW 2
#define FAT 3
#define THIN 4
#define BORDER 5

struct Point

{
	Point()

	{

	}

	Point(Vec2f position_, int type_, float radius_): position(position_), type(type_), radius(radius_)

	{

	}

	Vec2f position;
	int type;
	float radius;
};