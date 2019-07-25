#pragma once
#include <SFML/Graphics.hpp>
#include <Maths.h>

class Map
{
public:
	Map(int widthWin, int heightWin, int size, int thickness);
	int w, h, t, size;
	Vec2i origin;
	void update();
	void draw(sf::RenderWindow& window);
	void papersPleaseDisabled(int ch);
	bool flashyBoarder;
private:
	sf::Clock boarderControl, flashTimer;
	sf::RectangleShape winOuter;
	sf::RectangleShape winInner;
};