//Main class
#include "Game.h"

int main()
{
    Game* game = new Game();
	game->init();
	
	while (!game->isDone())
	{
		game->input();
		game->render();
	}
    
    return 0;
}
