#include "program.h"
#include <ctime>

int main()
{
	
	srand(clock());
	Program p(700, 700);
	return p.mainLoop();
}