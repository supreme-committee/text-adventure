//Main class
#include "Game.h"
#include "parser.h"
#include "Logger.h"

#include <sstream>
#include <string>
#include <iostream>
using namespace std;

int main(int argc, char** argv)
{
	// This is just some code for testing the parser. It'll be removed eventually. - michaelg
	/*std::map<std::string, bool> boolVars;
	std::map<std::string, int> intVars;
	std::map<std::string, std::string> stringVars;

	Tile t = Parser::parse("../Sample-Game/Concept-michaelg/Start.xml", boolVars, intVars, stringVars);
	*/

    Game* game = new Game();
	if (!game->init()) 
	{
		cerr << "Game initialization failed. See previous messages" << endl;
		return 1;
	}
	
	while (!game->isDone())
	{
		game->input();
		game->render();
	}
    
    Logger::log("HELLO\n");
    Logger::shutdown();
    return 0;
}
