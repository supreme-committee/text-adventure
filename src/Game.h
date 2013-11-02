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
	string currentFile; // The currently loaded xml file

	sf::Font font_main;
	sf::Text text; // The text to show (built from the texts vector in tile)
	vector<button> buttons; // The choices that'll be displayed to the user
	unsigned int buttonSelection;    // The currently selected button
	void buildText();     // Set up the text so it fits cleanly on the screen
	void createButtons(); // Create Button objects for links from the Tile
	void scrollButtonsUp();   // Scroll the buttons
	void scrollButtonsDown();
	void loadFile(string filename); // Load and set up a new tile from another xml file
	menu *m;

public:
	Game();
	~Game();
	bool init(string startFile);   // initialize compenents
	void input();  // Handle user input
	void update(); // Handle game logic
	void render(); // Render stuff on screen
	
	// inline functions
	inline bool isDone() const { return done; }
};

#endif
