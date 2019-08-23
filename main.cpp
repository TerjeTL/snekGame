#include "program.h"
#include <ctime>

int main()
{
	sf::err().rdbuf(NULL);

	srand(clock());
	Program p(700, 700);
	return p.mainLoop();
}