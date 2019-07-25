#include "program.h"
#include "snek.h"


Program::Program(int width, int height) : w(width), h(height), window(sf::VideoMode(width, height), "SFML Test"), area(width, height, height - 100, 5),
activatorSize(6), running(true)
{
	snakes.push_back(Snek(area));
	snakes[0].body.setFillColor(sf::Color::Green);
	/*snakes.push_back(Snek(area));
	snakes[1].body.setFillColor(sf::Color::Red);*/
}

int Program::mainLoop()
{
	sf::Event events;
	sf::Clock clockUpdate;

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
			update();
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

		draw();
		

		//Display
		window.display();
	}
}

void Program::checkFood()

{
	for (int i = 0; i < snakes.size(); i++)
	{
		for (int j = 0; j < snakes[i].points.size(); j++)
		{
			for (int k = 0; k < foodVec.size(); k++)
			{
				if (distance(snakes[i].points[j].position, foodVec[k].activatorPoint) < snakes[i].bodySize + activatorSize)
				{
					switch (foodVec[k].ident)
					{
					case 0:
						snakes[i].speedSnek(1);
						std::cout << "speed" << std::endl;
						break;

					case 1:
						snakes[i].speedSnek(-1);
						break;

					case 2: //fat snek
						snakes[i].fatSnek(1);
						break;

					case 3: //skinny snek
						snakes[i].fatSnek(-1);
						break;
					}
					foodVec.erase(foodVec.begin() + k);
				}
			}
		}
	}

	for (int i = 0; i < foodVec.size(); i++)
	{
		std::cout << i << std::endl;
	}

	if (spawnClock.getElapsedTime().asSeconds() > 3)
	{
		Vec2f position;
		position.x = randomInt(area.origin.x, area.origin.x + area.size);
		position.y = randomInt(area.origin.y, area.origin.y + area.size);

		int rand = randomInt(0, 3);

		if (rand == 0)
		{	//fass boii
			Food speedyBoi(0, position, activatorSize, sf::Color::Green);
			foodVec.push_back(speedyBoi);
		}
		else if (rand == 1)
		{ //slow boii
			Food slowBoi(1, position, activatorSize, sf::Color::Red);
			foodVec.push_back(slowBoi);
		}
		else if (rand == 2) //fat snek
		{
			Food fatBoi(2, position, activatorSize, sf::Color::Magenta);
			foodVec.push_back(fatBoi);
		}
		else if (rand == 3)
		{	//skinny snek
			Food skinnyBoi(3, position, activatorSize, sf::Color::Cyan);
			foodVec.push_back(skinnyBoi);
		}


		spawnClock.restart();
	}
}


void Program::update()
{
	for (int i = 0; i < snakes.size(); i++)
	{
		snakes[i].update(snakes, area, i);
	}

	checkFood();
}

void Program::draw()
{
	area.draw(window);

	for (int i = 0; i < snakes.size(); i++)
	{
		snakes[i].draw(window, snakes, area);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && snakes[i].snekRekt)
		{
			window.clear(sf::Color::Black);
			foodVec.clear();
			for (int i = 0; i < snakes.size(); i++)
			{
				snakes[i].points.clear();
				snakes[i].resetPos(area);
				snakes[i].snekRekt = false;
			}
			break;
		}
	}

	for (int i = 0; i < foodVec.size(); i++)
	{
		window.draw(foodVec[i].activator);
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

int Program::randomInt(int min, int max)
{
	return rand() % (max - min + 1) + min;
}