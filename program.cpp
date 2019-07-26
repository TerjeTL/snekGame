#include "program.h"


Program::Program(int width, int height) : w(width), h(height), area(width, height, height - 100, 5),
activatorSize(6), running(true), snek(area, networkHandler), networkHandler(mtx, ghosts, snek.points), window(sf::VideoMode(width, height), "Sneky boi")
{
	snek.body.setFillColor(sf::Color::Green);
	//ghosts.push_back(Ghost());
	/*snakes.push_back(Snek(area));
	snakes[1].body.setFillColor(sf::Color::Red);*/

	//std::cout << colors[0][1] << std::endl;
}

int Program::mainLoop()
{
	sf::Event events;
	sf::Thread* thread = 0;
	//NetworkHandler 
	networkHandler.connect("82.47.120.89", 5000);
	networkHandler.sendCreate();
	if (!window.isOpen())
	{
		return EXIT_FAILURE;
	}

	thread = new sf::Thread(&Program::updateThread, this);
	thread->launch();

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


		//if (!snakes[0].snekRekt && !snakes[1].snekRekt) running = true;
		
		//clear
		window.clear(sf::Color::Black);
		//Draw
		draw();
		//Display
		window.display();
	}
	quit = 1;
	if (thread)

	{
		thread->wait();
		delete thread;
	}

	networkHandler.quitConnection();
	std::cout << "Shutting down" << std::endl;
	return EXIT_SUCCESS;
}

void Program::updateThread()

{
	sf::Clock clockUpdate;
	while (!quit)

	{
		if (clockUpdate.getElapsedTime().asSeconds() >= 1.0 / 60.0 && running)
		{
			update();
			clockUpdate.restart();
		}
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

	for (int i = 0; i < ghosts.size(); i++)

	{
		ghosts[i].update();
	}

	spawnFood();
}

void Program::draw()
{
	mtx.lock();
	area.draw(window);
	snek.draw(window);

	for (int i = 0; i < ghosts.size(); i++)

	{
		ghosts[i].draw(window);
	}

	sf::CircleShape food(activatorSize);
	food.setOrigin(activatorSize, activatorSize);

	for (int i = 0; i < foods.size(); i++)

	{
		sf::Color color(colors[foods[i].type - 1][0], colors[foods[i].type - 1][1], colors[foods[i].type - 1][2]);
		food.setFillColor(color);
		food.setPosition(foods[i].position.x, foods[i].position.y);
		window.draw(food);
	}
	mtx.unlock();
}

void Program::reset()

{
	if (snek.snekRekt == true)

	{
		snek.points.clear();
		snek.resetPos(area);
		snek.snekRekt = false;
		running = true;
		foods.clear();
		networkHandler.sendClear();
	}
}

void Program::eventHandler(sf::Event events)
{

	float rot = (float)0.0;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) rot--;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) rot++;

	if (!snek.squareSnek || snek.squareSnekTimer.getElapsedTime().asSeconds() > 10)
	{	
		snek.setRotAngle(rot);
		snek.squareSnek = false;
	}
	else
	{
		snek.setRotAngle(PI/2 *rot);
	}


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