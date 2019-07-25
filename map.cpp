#include "map.h"

Map::Map(int widthWin, int heightWin, int size, int thickness) : w(widthWin), h(heightWin), t(thickness), size(size), winOuter(sf::Vector2f(size+thickness, size+thickness)), winInner(sf::Vector2f(size, size)), flashyBoarder(false)
{
	flashingTime = 10; //seconds
	flasher = 200; //ms
	winOuter.setFillColor(sf::Color::White);
	winInner.setFillColor(sf::Color::Black);
	winOuter.setPosition((widthWin - (size+thickness)) / 2, (heightWin - (size + thickness)) / 2);
	winInner.setPosition((widthWin - size) / 2, (heightWin - size) / 2);
	origin.x = (widthWin - size) / 2;
	origin.y = (heightWin - size) / 2;
	//std::cout << origin << std::endl;
}

void Map::draw(sf::RenderWindow& window)
{
	if (flashyBoarder)
	{
		int flasherCopy = flasher;
		if (boarderControl.getElapsedTime().asSeconds() > flashingTime-2) flasherCopy = flasherCopy/2;
		
		if (flashTimer.getElapsedTime().asMilliseconds() >= flasherCopy)
		{
			window.draw(winOuter);
			window.draw(winInner);
			if (flashTimer.getElapsedTime().asMilliseconds() >= flasherCopy*4) flashTimer.restart();
		}
	}
	else
	{
		window.draw(winOuter);
		window.draw(winInner);
	}
}

void Map::papersPleaseDisabled(int ch)
{
	if (ch == 1)
	{
		flashyBoarder = true;
		boarderControl.restart();
	}
	else if (ch == 0 && boarderControl.getElapsedTime().asSeconds() > flashingTime && flashyBoarder == true) flashyBoarder = false;
}