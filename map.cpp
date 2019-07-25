#include "map.h"

Map::Map(int widthWin, int heightWin, int size, int thickness) : w(widthWin), h(heightWin), t(thickness), size(size), winOuter(sf::Vector2f(size+thickness, size+thickness)), winInner(sf::Vector2f(size, size))
{
	winOuter.setFillColor(sf::Color::White);
	winInner.setFillColor(sf::Color::Black);
	winOuter.setPosition((widthWin - (size+thickness)) / 2, (heightWin - (size + thickness)) / 2);
	winInner.setPosition((widthWin - size) / 2, (heightWin - size) / 2);
	origin.x = (widthWin - size) / 2;
	origin.y = (heightWin - size) / 2;
	std::cout << origin << std::endl;
}

void Map::draw(sf::RenderWindow& window)
{
	window.draw(winOuter);
	window.draw(winInner);
}