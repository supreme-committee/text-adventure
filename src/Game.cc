#include "Game.h"
#include "parser.h"
#include <iostream>
using namespace std;

// ================== PRIVATE FUNCTIONS ==================
void Game::buildText()
{
	// Take strings from tile.texts and build a single sf::Text object from it
}
void Game::createButtons()
{
	// Create a button for each link in this tile
	for (auto& link : tile.links)
	{
		//Button button(font_main, link.text, link.filename);
		//buttons.push_back(button);
	}
}

// ================== PUBLIC FUNCTIONS ===================
Game::Game()
{
	done = false;
	window.create(sf::VideoMode(640, 480), "Game Engine", sf::Style::Close);
	window.setFramerateLimit(30);
}
Game::~Game()
{
}
bool Game::init()
{
	/*if (!font_main.loadFromFile("arial.ttf")) // Load the font. We need to figure out what kind of font to use - michaelg
	{
		cerr << "ERROR: Could not load font file" << endl;
		return false;
	}*/

	tile = Parser::parse("../Sample-Game/Concept-michaelg/Start.xml", boolVars, intVars, stringVars); // Get the starting tile
	if (tile.links.size() == 0) // No links exist. Parsing failed
	{
		cerr << "Error occurred while parsing start.xml" << endl;
		return false;
	}

	buildText();
	createButtons();

	return true;
}
void Game::input()
{
	sf::Event ev;
	while (window.pollEvent(ev))
	{
		if (ev.type == sf::Event::Closed) done = true;
		else if (ev.type == sf::Event::MouseButtonPressed)
		{
			if (ev.mouseButton.button == sf::Mouse::Left)
			{
				// Figure out what button was pressed, and trigger that button's click() function
				// when clicked, get the next file, and send it to the parser which will return a new Tile object
			}
		}
	}
}
void Game::render()
{
	window.clear(sf::Color::Black);

	window.draw(text);
	/*for (auto& button : buttons) // render the buttons
	{
		window.draw(button);
	}*/

	window.display();
}