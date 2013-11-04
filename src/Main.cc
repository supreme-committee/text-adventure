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
	//thread t(Logger::write); // Create a thread for the writer
	//t.detach(); // Lets thread run in background (also prevents abort() being called when thread is destroyed)

	if (argc < 2 || argc > 3)
	{
		Logger::log("ERROR: you must specify the game tar on the command line");
		Logger::log("Usage: game-engine.exe [tar file]");
		cerr << "Usage: game-engine.exe [tar file]" << endl;
		return 1;
	}

	if (argc == 3 && strcmp(argv[2], "-l") == 0) // If user wants logging turned on
	{
		cout << "logging turned on" << endl;
		Logger::init();
	}

    Game game;
	if (!game.init(argv[1])) 
	{
		Logger::log("Game initialization failed. See previous messages");
		return 1;
	}
	
	while (!game.isDone())
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
