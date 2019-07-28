#pragma once
#include <SFML/Graphics.hpp>
#include <Maths.h>
#include <vector>
#include "Ghost.h"

class Map
{
public:
	Map(int widthWin, int heightWin, int size, int thickness, std::vector<Ghost>& ghosts_);
	int w, h, t, size;
	Vec2i origin;
	void update();
	void draw(sf::RenderWindow& window);
	void papersPleaseDisabled(int ch);
	bool flashyBoarder;
	sf::Color snakeColor;
	std::string* name;
	int ready = 0;
private:
	int flashingTime, flasher;
	sf::Clock boarderControl, flashTimer;
	sf::RectangleShape winOuter;
	sf::RectangleShape winInner;
	std::vector<Ghost>& ghosts;

	sf::Font font;
	sf::Text text;
	sf::Text ghostText;
	sf::Sprite tick;
	sf::Sprite cross;

	sf::Texture tickTexture;
	sf::Texture crossTexture;
};