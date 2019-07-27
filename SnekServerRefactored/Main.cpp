#include "Server.h"
#include <ctime>
int main(int argc, char** argv)

{
	srand(clock());
	Server server;
	return server.mainLoop();
}