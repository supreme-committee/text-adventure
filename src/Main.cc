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
	thread t(Logger::write); // Create a thread for the writer
	t.detach(); // Lets thread run in background (also prevents abort() being called when thread is destroyed)

	// This is just some code for testing the parser. It'll be removed eventually. - michaelg
	/*std::map<std::string, bool> boolVars;
	std::map<std::string, int> intVars;
	std::map<std::string, std::string> stringVars;

	Tile t = Parser::parse("../Sample-Game/Concept-michaelg/Start.xml", boolVars, intVars, stringVars);
	*/

    /*Game game;
	if (!game.init()) 
	{
		cerr << "Game initialization failed. See previous messages" << endl;
		return 1;
	}
	
	while (!game.isDone())
	{
		game.input();
		game.render();
	}*/
    
	// Code for testing Logger (remove when done)
    Logger::log("HELLO");
	Logger::log("I have been logged!");
	Logger::log("Ahhhhhhhhhhhhhhhhhhhhh!!!");
	
	Logger::shutdown(); // Tell the logger it's time to shutdown its thread
	while (Logger::active()) // Wait until Logger is done emptying the queue
	{
		this_thread::sleep_for(chrono::milliseconds(100));
	}
	
    return 0;
}
