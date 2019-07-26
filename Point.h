#pragma once
#include <Maths.h>

#define SNAKE 0
#define FAST 1
#define SLOW 2
#define FAT 3
#define THIN 4
#define BORDER 5
#define SQUARE 6
#define WOOSH 7
#define REVSNEK 8
#define MITOSIS 9

struct Point

{
	Point()

	{

	}

	Point(Vec2f position_, int type_, float radius_): position(position_), type(type_), radius(radius_)

	{

	}

	Point(Vec2f position_, int type_, float radius_, int id_) : position(position_), type(type_), radius(radius_), id(id_)

	{

	}

	Vec2f position;
	int type;
	float radius;
	int id = -1;
};