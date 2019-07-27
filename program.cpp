#include "program.h"


Program::Program(int width, int height) : w(width), h(height), area(width, height, height - 100, 5),
activatorSize(6), running(true), snek(area, networkHandler, mtx, qtree), networkHandler(mtx, ghosts, snek.points), window(sf::VideoMode(width, height), "Sneky boi"),
spawnTimer(4)
{
	snek.body.setFillColor(sf::Color::Green);
	//ghosts.push_back(Ghost());
	/*snakes.push_back(Snek(area));
	snakes[1].body.setFillColor(sf::Color::Red);*/

	qtree = new QuadTree(Boundary(width, height, width / 2, height / 2), 10);

	//std::cout << colors[0][1] << std::endl;
}

Program::~Program()

{
	if (qtree != nullptr)

	{
		delete qtree;
	}
}

int Program::mainLoop()
{
	sf::Event events;
	sf::Clock clockUpdate;
	//NetworkHandler 
	networkHandler.connect("82.47.120.89", 5000);
	networkHandler.sendCreate();
	
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
		//std::cout << "wait" << std::endl;
		draw();
		//std::cout << "wait" << std::endl;
		//Display
		window.display();
	}

	networkHandler.quitConnection();
	std::cout << "Shutting down" << std::endl;
	return EXIT_SUCCESS;
}

void Program::spawnFood(Snek snek) // i have no idea if this mitosis stuff will work with networking

{
	int spawnTimerCopy = spawnTimer;
	if (snek.mitosisClock.getElapsedTime().asSeconds() < 4 && snek.mitosis) spawnTimerCopy = 1;
	else snek.mitosis = false;

	if (spawnClock.getElapsedTime().asSeconds() > spawnTimerCopy)
	{
		Vec2f position;
		position.x = randomInt(area.origin.x, area.origin.x + area.size);
		position.y = randomInt(area.origin.y, area.origin.y + area.size);
		int type = randomInt(1, 10);
		int id = foods.size();
		foods.push_back(Point(position, type, activatorSize, id));
		spawnClock.restart();
	}
}


void Program::update()
{
	if (qtree != nullptr) delete qtree;
	qtree = new QuadTree(Boundary(w, h, w / 2, h / 2), 10);
	int loop = 0;
	if (snek.points.size() > 10) loop = 1;

	for (int i = 0; i < snek.points.size() - 10 && loop; i++)

	{
		qtree->insert(Point(snek.points[i].position, snek.points[i].type, snek.points[i].radius));
	}

	for (int i = 0; i < ghosts.size(); i++)

	{
		for (int j = 0; j < ghosts[i].points.size(); j++)

		{
			qtree->insert(Point(ghosts[i].points[j].position, ghosts[i].points[j].type, ghosts[i].points[j].radius));
		}
	}

	for (int i = 0; i < foods.size(); i++)

	{
		qtree->insert(Point(foods[i].position, foods[i].type, foods[i].radius, foods[i].id));
	}

	if (snek.snekRekt)

	{
		running = false;
	}
	snek.update(ghosts, area, foods);

	for (int i = 0; i < ghosts.size(); i++)

	{
		ghosts[i].update();
	}
	
	spawnFood(snek);
}

void Program::draw()
{
	//mtx.lock();
	area.draw(window);
	snek.draw(window);
	mtx.lock();
	std::vector<Ghost> ghostsCopy = ghosts;
	mtx.unlock();
	for (int i = 0; i < ghostsCopy.size(); i++)

	{
		ghostsCopy[i].draw(window);
	}

	//qtree->draw(window);

	sf::CircleShape food(activatorSize);
	food.setOrigin(activatorSize, activatorSize);
	//mtx.lock();
	for (int i = 0; i < foods.size(); i++)

	{
		sf::Color color(colors[foods[i].type - 1][0], colors[foods[i].type - 1][1], colors[foods[i].type - 1][2]);
		food.setFillColor(color);
		food.setPosition(foods[i].position.x, foods[i].position.y);
		window.draw(food);
	}
	//mtx.unlock();
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

void Program::eventHandler(sf::Event events) // This needs a rework. Especially the revsnek solution is messy. should be simple to make it at least 100% better. later....
{
	float rot = (float)0.0;

	if (!snek.squareSnek || snek.squareSnekTimer.getElapsedTime().asSeconds() > 10)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) rot--;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) rot++;
		snek.setRotAngle(rot);
		snek.squareSnek = false;
	}
	else
	{
		if (events.type == sf::Event::KeyPressed)
		{
			switch (events.key.code)
			{
			case sf::Keyboard::A:
			{
				if (snek.revSnek) snek.velocity.rotateInPlaze(PI / 2);
				else snek.velocity.rotateInPlaze(-PI / 2);
				break;
			}
			case sf::Keyboard::D:
			{
				if (snek.revSnek) snek.velocity.rotateInPlaze(-PI / 2);
				else snek.velocity.rotateInPlaze(PI / 2);
				break;
			}
			}
		}
		snek.setRotAngle(rot);
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