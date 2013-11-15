//Game.h
#ifndef _GAME_H_
#define _GAME_H_

#include <string>
#include <map>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#ifdef WIN32
#include <Windows.h>
#endif
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
	const string fileDirectory; // The directory where the story files are
	string currentFile; // The currently loaded xml file
	string tarFile;     // The name of the loaded .tar file
	string currentMusic;	//Keep track of music file being played.

	sf::Font font_main;
	vector<sf::Text> texts; // The text to show (built from the texts vector in tile)
	unsigned int textSelection; // The currently selected (i.e. most visible) line of text

	vector<button> buttons; // The choices that'll be displayed to the user
	unsigned int buttonSelection;    // The currently selected button
	void buildText();     // Set up the text so it fits cleanly on the screen
	void createButtons(); // Create Button objects for links from the Tile

	void scrollTextUp();      // Scroll the text
	void scrollTextDown(); 
	void scrollButtonsUp();   // Scroll the buttons
	void scrollButtonsDown();
	void loadFile(string filename); // Load and set up a new tile from another xml file
	menu *m;
	sf::Texture texture_background; // The texture for the background image
	sf::Sprite sprite_background;	// The sprite (a sf::Drawable object) for the background image
	sf::SoundBuffer soundbuffer;	//Stores the sound that will play from the tile
	sf::Sound sound;				//Plays the sound specified in a tile
	sf::Music bgm;					//Background music
	bool imageValid, hideUI, muteButtons;
	bool setup(); // Load font, button icon, and other basic initialization stuff
	void setupNewGame(string tarFile); // Extract the given tar, set up new game, and load start.xml

public:
	Game();
	~Game();
	bool init(); // used when user does not specify a tar file
	bool init(string startFile);   // initialize game with given tar file
	void input();  // Handle user input
	void update(); // Handle game logic
	void render(); // Render stuff on screen
	void rescaleImage(); //Rescale an image to 640x480, keeping aspect ratio of original.
	void loadMusic(string);
	
	// inline functions
	inline bool isDone() const { return done; }
	inline void showErrorMessage(string message)
	{
#ifdef WIN32
		MessageBoxA(NULL, message.c_str(), "Error", MB_OK | MB_ICONEXCLAMATION);
#endif
#ifdef __APPLE__
        message = message;
#endif
	}
};

#endif
