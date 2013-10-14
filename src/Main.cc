//Main class
#include "Game.h"
#include "parser.h"
#include "Logger.h"

#include <sstream>
#include <string>
#include <iostream>
#include <thread>
using namespace std;

int main(int argc, char** argv)
{
	Logger::init();
	//thread t(Logger::write); // Create a thread for the writer
	//t.detach(); // Lets thread run in background (also prevents abort() being called when thread is destroyed)

    Game game;
	if (!game.init()) 
	{
		Logger::log("Game initialization failed. See previous messages");
		return 1;
	}
	
	while (!game.isDone())
	{
		game.input();
		game.render();
	}
    
	//Logger::shutdown(); // Tell the logger it's time to shutdown its thread
	/*while (Logger::active()) // Wait until Logger is done emptying the queue
	{
		this_thread::sleep_for(chrono::milliseconds(100));
	}*/
	
    return 0;
}
