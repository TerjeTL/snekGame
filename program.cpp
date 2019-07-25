#include "program.h"
#include "snek.h"


Program::Program(int width, int height) : w(width), h(height), window(sf::VideoMode(width, height), "SFML Test"), area(width, height, height - 100, 5),
activatorSize(6), running(true), snek(area)
{
	snek.body.setFillColor(sf::Color::Green);
	/*snakes.push_back(Snek(area));
	snakes[1].body.setFillColor(sf::Color::Red);*/

	//std::cout << colors[0][1] << std::endl;
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
		//if (!snakes[0].snekRekt && !snakes[1].snekRekt) running = true;
		
		//clear
		window.clear(sf::Color::Black);
		//Draw
		draw();
		//Display
		window.display();
	}
}

void Program::spawnFood()

{

	if (spawnClock.getElapsedTime().asSeconds() > 3)
	{
		Vec2f position;
		position.x = randomInt(area.origin.x, area.origin.x + area.size);
		position.y = randomInt(area.origin.y, area.origin.y + area.size);
		int type = randomInt(1, 5);

		foods.push_back(Point(position, type, activatorSize));
		spawnClock.restart();
	}
}


void Program::update()
{
	if (snek.snekRekt)

	{
		running = false;
	}
	
	snek.update(ghosts, area, foods);

	spawnFood();
}

void Program::draw()
{
	area.draw(window);
	snek.draw(window);

	sf::CircleShape food(activatorSize);
	food.setOrigin(activatorSize, activatorSize);

	for (int i = 0; i < foods.size(); i++)

	{
		sf::Color color(colors[foods[i].type - 1][0], colors[foods[i].type - 1][1], colors[foods[i].type - 1][2]);
		food.setFillColor(color);
		food.setPosition(foods[i].position.x, foods[i].position.y);
		window.draw(food);
	}
}

void Program::reset()

{
	if (snek.snekRekt == true)

	{
		snek.points.clear();
		snek.resetPos(area);
		snek.snekRekt = false;
		foods.clear();
	}
}

void Program::eventHandler(sf::Event events)
{

	float rot = (float)0.0;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) rot--;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) rot++;

	snek.setRotAngle(rot);

	if (events.type == sf::Event::EventType::KeyPressed)

	{
		if (events.key.code == sf::Keyboard::Space)

		{
			reset();
		}
	}
}

int Program::randomInt(int min, int max)
{
	return rand() % (max - min + 1) + min;
}