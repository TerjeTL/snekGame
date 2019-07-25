#include "program.h"
#include <ctime>

int main()
{
	
	srand(clock());
	Program p(900, 900);
	return p.mainLoop();
}