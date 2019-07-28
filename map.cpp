#include "map.h"

Map::Map(int widthWin, int heightWin, int size, int thickness, std::vector<Ghost>& ghosts_) :
	w(widthWin), h(heightWin), t(thickness), size(size), winOuter(sf::Vector2f(size+thickness, size+thickness)), 
	winInner(sf::Vector2f(size, size)), flashyBoarder(false), ghosts(ghosts_)
{
	flashingTime = 10; //seconds
	flasher = 200; //ms
	winOuter.setFillColor(sf::Color::White);
	winInner.setFillColor(sf::Color::Black);
	winOuter.setPosition((widthWin - (size+thickness)) / 2, (heightWin - (size + thickness)) / 2);
	winInner.setPosition((widthWin - size) / 2, (heightWin - size) / 2);
	origin.x = (widthWin - size) / 2;
	origin.y = (heightWin - size) / 2;
	font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");
	ghostText.setFont(font);
	ghostText.setCharacterSize(20);
	ghostText.setString("Test");
	//ghostText.setOrigin(0, ghostText.getGlobalBounds().height);
	text.setFont(font);
	text.setCharacterSize(30);
	text.setString("Test");
	//text.setOrigin(0, text.getGlobalBounds().height);
	text.setPosition(sf::Vector2f(50, heightWin - 50));
	//std::cout << origin << std::endl;

	tickTexture.loadFromFile("Resources/Tick.png");
	crossTexture.loadFromFile("Resources/Cross.png");

	tick.setTexture(tickTexture);
	cross.setTexture(crossTexture);

	tick.setOrigin(0, tick.getLocalBounds().height / 2);
	cross.setOrigin(0, cross.getLocalBounds().height / 2);
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
	text.setFillColor(snakeColor);
	text.setString(*name);
	window.draw(text);
	

	if (ready) tick.setScale(1.5, 1.5), tick.setPosition(50 + text.getLocalBounds().width + 10, h - 50 + text.getLocalBounds().height/2 + 10), window.draw(tick);
	else cross.setScale(1.5, 1.5), cross.setPosition(50 + text.getLocalBounds().width + 10, h - 50 + text.getLocalBounds().height / 2 + 10), window.draw(cross);

	int prev = 0;
	int y = 0;

	tick.setScale(1.0,1.0);
	cross.setScale(1.0,1.0);
	

	for (int i = 0; i < ghosts.size(); i++)

	{
		//ghostText.setOrigin(0, ghostText.getGlobalBounds().height*2);
		if (prev >= 800)

		{
			prev = 800;
			y = ghostText.getGlobalBounds().height;
		}
		int xpos = 50 + (prev % 800);
		ghostText.setPosition(xpos, y);
		ghostText.setString(ghosts[i].name);
		ghostText.setFillColor(ghosts[i].color);

		if (ghosts[i].ready) tick.setPosition(xpos + ghostText.getGlobalBounds().width + 10, y + ghostText.getLocalBounds().height / 2 + 6), window.draw(tick);
		else cross.setPosition(xpos + ghostText.getGlobalBounds().width + 10, y + ghostText.getLocalBounds().height / 2 + 6), window.draw(cross);

		window.draw(ghostText);
		prev += ghostText.getGlobalBounds().width + 30;
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