//Game.h
#ifndef _GAME_H_
#define _GAME_H_

#include <string>
#include <map>
#include <SFML/Graphics.hpp>

class Game
{
private:
	sf::RenderWindow window;
	bool done;
	std::map<std::string, bool> boolVariables;
	//Tile tile; // The current tile's info

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
