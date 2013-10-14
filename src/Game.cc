#include "Game.h"
#include "parser.h"
#include "Logger.h"
#include <iostream>
#include <sstream>
using namespace std;

// ================== PRIVATE FUNCTIONS ==================
void Game::buildText()
{
	ostringstream stringStream;
	for (string& s : tile.texts)
	{
		stringStream << s;
	}
	text.setString(stringStream.str());
}
void Game::createButtons()
{
	float yPos = 250.0f;
	// Create a button for each link in this tile
	for (auto& link : tile.links)
	{
		button b(link, font_main, 30.0f, yPos);
		buttons.push_back(b);
		yPos += 60.0f;
	}
}
void Game::loadFile(string filename)
{
	text.setString("");
	buttons.clear();

	tile = Parser::parse(filename.c_str(), boolVars, intVars, stringVars);
	buildText();
	createButtons();
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
	if (!font_main.loadFromFile("arial.ttf")) // Load the font. We need to figure out what kind of font to use - michaelg
	{
		Logger::log("ERROR: could not load font file");
		return false;
	}

	text.setFont(font_main);
	text.setCharacterSize(32);
	text.setColor(sf::Color::Black);
	text.setPosition(sf::Vector2f(10.0f, 10.0f));

	// Iterate through input files and verify they are formatted correctly
	if (!Parser::verify("../Sample-Game/Concept-michaelg/Start.xml"))
	{
		return false;
	}
	
	tile = Parser::parse("../Sample-Game/Concept-michaelg/Start.xml", boolVars, intVars, stringVars); // Get the starting tile
	if (tile.links.size() == 0 && tile.texts.size() == 0) // No links or text exists. wtf
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
				for (auto& b : buttons)
				{
					/*if (b.isMouseOver(ev.mouseButton.x, ev.mouseButton.y))
					{
						loadFile(b.getLink()); // Load the next xml file
						break;
					}*/
				}
			}
		}
	}
}

void Game::render()
{
	window.clear(sf::Color::Cyan);

	window.draw(text);
	for (auto& button : buttons) // render the buttons
	{
		button.render(window);
	}

	window.display();
}