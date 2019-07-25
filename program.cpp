#include "program.h"
#include "snek.h"
#include "functions.h"


Program::Program(int width, int height) : w(width), h(height), window(sf::VideoMode(width, height), "SFML Test"), area(width, height, height - 100, 5)
{
	snakes.push_back(Snek(area));
	snakes[0].body.setFillColor(sf::Color::Green);
	/*snakes.push_back(Snek(area));
	snakes[1].body.setFillColor(sf::Color::Red);*/
	running = true;
}

int Program::mainLoop()
{
	sf::Event events;
	sf::Clock clockUpdate;
	Functions gameEvents;

	if (!window.isOpen())
	{
		return EXIT_FAILURE;
	}

	while (window.isOpen())
	{
		while (window.pollEvent(events))
		{
			if (events.type == sf::Event::EventType::Closed)
			{
				window.close();
			}

			eventHandler(events);
		}

		if (clockUpdate.getElapsedTime().asSeconds() >= 1.0 / 60.0 && running)
		{
			for (int i = 0; i < snakes.size(); i++)
			{
				snakes[i].update(snakes, snakes[i], area);
			}

			gameEvents.update(area, snakes);

			clockUpdate.restart();
		}
		
		int check = 0;
		for (int j = 0; j < snakes.size(); j++)
		{
			if (snakes[j].snekRekt)
			{
				running = false;
			}
			else
			{
				check ++;
				if (check == snakes.size())
				{
					running = true;
				}
			}
		}

		//if (!snakes[0].snekRekt && !snakes[1].snekRekt) running = true;
		
		//clear
		window.clear(sf::Color::Black);

		//Features
		

		//Draw

		area.draw(window);
		
		gameEvents.draw(window);

		for (int i = 0; i < snakes.size(); i++)
		{
			snakes[i].draw(window, snakes, area);

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && snakes[i].snekRekt)
			{
				window.clear(sf::Color::Black);
				gameEvents.foodVec.clear();
				for (int i = 0; i < snakes.size(); i++)
				{
					snakes[i].points.clear();
					snakes[i].resetPos(area);
					snakes[i].snekRekt = false;
				}
				break;
			}
		}
		

		//Display
		window.display();
	}
}

void Program::eventHandler(sf::Event events)
{
	for (int i = 0; i < snakes.size(); i++)
	{
		float rot = (float)0.0;
		if (i == 0)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) rot--;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) rot++;
		}
		else
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) rot--;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) rot++;
		}
		
		snakes[i].setRotAngle(snakes[i].rotSpeed*rot);
	}
	
	
}