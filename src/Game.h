//Game.h
#ifndef _GAME_H_
#define _GAME_H_

#include <string>
#include <map>
#include <SFML/Graphics.hpp>
#include "tile.h"

class Game
{
private:
	sf::RenderWindow window;
	bool done;
	map<string, bool> boolVars;
	map<string, int> intVars;
	map<string, string> stringVars;
	Tile tile; // The current tile's info

	sf::Font font_main;
	sf::Text text; // The text to show (built from the texts vector in tile)
	//vector<Button> buttons; // The choices that'll be displayed to the user
	void buildText();
	void createButtons(); // Create Button objects for links from the Tile

public:
	Game();
	~Game();
	bool init();   // initialize compenents
	void input();  // Handle user input
	void render(); // Render stuff on screen
	
	// inline functions
	inline bool isDone() const { return done; }
};

#endif
