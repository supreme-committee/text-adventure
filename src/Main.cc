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
    
    system("rm .gamefiles/*");
    
	//thread t(Logger::write); // Create a thread for the writer
	//t.detach(); // Lets thread run in background (also prevents abort() being called when thread is destroyed)

	if (argc > 3)
	{
		cout << "Usage: text-adventure.exe [.tar file] [-l]" << endl;
		return 1;
	}

	Game game;
	string tarFile; // The tar file the user provided
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

	if (tarFile.length() > 0) // Initialize game with given tar file
	{
		if (!game.init(tarFile)) game.init();
	}
	else // initialze game without starting file
	{
		if (!game.init()) return 1;
	}
	
	while (!game.isDone()) // Run the game loop
	{
		game.input();
		game.update();
		game.render();
	}
    
	//Logger::shutdown(); // Tell the logger it's time to shutdown its thread
	/*while (Logger::active()) // Wait until Logger is done emptying the queue
	{
		this_thread::sleep_for(chrono::milliseconds(100));
	}*/
	
    return 0;
}
