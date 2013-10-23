//Game.h
#ifndef _GAME_H_
#define _GAME_H_

#include <string>
#include <map>
#include <SFML/Graphics.hpp>
#include "tile.h"
#include "button.h"
#include "menu.h"

class Game
{
private:
	sf::RenderWindow window;
	bool done;					// Is the game finished?
	map<string, bool> boolVars;
	map<string, int> intVars;
	map<string, string> stringVars;
	Tile tile; // The current tile's info
	string fileDirectory; // The directory where the story files are

	sf::Font font_main;
	sf::Text text; // The text to show (built from the texts vector in tile)
	vector<button> buttons; // The choices that'll be displayed to the user
	int buttonSelection;    // The currently selected button
	void buildText();
	void createButtons(); // Create Button objects for links from the Tile
	void loadFile(string filename); // Load and set up a new tile from another xml file
	menu *m;

public:
	Game();
	~Game();
	bool init(string startFile);   // initialize compenents
	void input();  // Handle user input
	void render(); // Render stuff on screen
	
	// inline functions
	inline bool isDone() const { return done; }
};

#endif
