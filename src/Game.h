#ifndef _GAME_H_
#define _GAME_H_

#include <SFML\Graphics.hpp>

class Game
{
private:
	sf::RenderWindow window;
	bool done;

public:
	Game();
	~Game();
	void init();   // initialize compenents
	void input();  // Handle user input
	void render(); // Render stuff on screen
	
	// inline functions
	inline bool isDone() { return done; }
};

#endif