//Main class
#include "Game.h"
#include "parser.h"

#include <sstream>
#include <string>

int main()
{
	std::map<std::string, bool> boolVars;
	std::map<std::string, int> intVars;
	std::map<std::string, std::string> stringVars;

	tile t = Parser::parse("../Sample-Game/Concept-michaelg/Start.xml", boolVars, intVars, stringVars);


    Game* game = new Game();
	game->init();
	
	while (!game->isDone())
	{
		game->input();
		game->render();
	}
    

    return 0;
}
