//Main class
#include "Game.h"
#include "parser.h"
#include "Logger.h"

#include <sstream>
#include <string>
#include <string.h>
#include <iostream>
#include <thread>
using namespace std;

int main(int argc, char** argv)
{
#ifdef WIN32
	system("rmdir .gamefiles /s /q");
#else
    system("rm .gamefiles/*");
#endif
    
	if (argc > 3)
	{
		cout << "Usage: text-adventure.exe [.tar file] [-l]" << endl;
		return 1;
	}

	Game game;
	string tarFile = " "; // The tar file the user provided
	for (int i = 1; i < argc; i++)
	{
		string arg(argv[i]);
		if (arg == "-l") Logger::init();
		else if (arg.substr(arg.length() - 4, arg.length() - 1) == ".tar")
		{
			tarFile = argv[i];
		}
		else
		{
			cerr << "Invalid argument: " << argv[i] << endl;
			return 1;
		}
	}

	game.init(tarFile); // Initialize the game engine
	
	while (!game.isDone()) // Run the game loop
	{
		game.input();
		game.update();
		game.render();
	}
	
    return 0;
}
