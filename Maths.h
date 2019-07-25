#pragma once
#include "Vec2.h"

template <class T> T magnitude(const Vec2<T>& v)

{
	return sqrt(v*v);
}

template <class T> Vec2<T> normalize(const Vec2<T>& v)

{
	T mag = magnitude(v);
	Vec2<T> normalized = v / mag;
	return normalized;
}

template <class T> Vec2<T> rotate(const Vec2<T>& v, const T& angle)

{
	return Vec2<T>(v.x*cos(angle) - v.y*sin(angle), v.x*sin(angle) + v.y*cos(angle));
}

template <class T> T distanceSquared(const Vec2<T>& v1, const Vec2<T>& v2)

{
	return pow(v1.x - v2.x, 2) + pow(v1.y - v2.y, 2);
}

template <class T> T distance(const Vec2<T>& v1, const Vec2<T>& v2)

{
	return sqrt(distanceSquared(v1, v2));
}

