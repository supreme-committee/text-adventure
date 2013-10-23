#include "Game.h"
#include "parser.h"
#include "Logger.h"
#include "FileHandler.h"
#include <iostream>
#include <sstream>
#include <fstream>
using namespace std;

// ================== PRIVATE FUNCTIONS ==================
void Game::buildText()
{
	ostringstream stringStream;
	for (string& s : tile.texts)
	{
		stringStream << s;
	}
	string textString = stringStream.str();

	int charCount = 0; // Now insert line breaks
	for (int pos = 0; pos < textString.length(); pos++)
	{
		if (charCount > 30 && textString[pos] == ' ')
		{
			textString.replace(pos, 1, "\n");
			charCount = 0;
		}
		charCount++;
	}

	text.setString(textString);
}
void Game::createButtons()
{
	float yPos = 250.0f;
	// Create a button for each link in this tile
	for (auto& link : tile.links)
	{
		button b(link, font_main, 30.0f, yPos);
		buttons.push_back(b);
		yPos += b.getHeight() + 10.0f;
	}
}
void Game::loadFile(string filename)
{
	text.setString(" ");
	buttons.clear();

	// Get full path to xml file
	string filePath = fileDirectory + ("/" + filename);

	// Check if file actually exists
	ifstream file;
	file.open(filePath, ios::in);
	if (!file.is_open())
	{
		Logger::log("ERROR: cannot find file " + filename);
		exit(1);
	}
	file.close();

	tile = Parser::parse(filePath.c_str(), boolVars, intVars, stringVars);
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
	delete m;
}
bool Game::init(string filename)
{
	if (!font_main.loadFromFile("arial.ttf")) // Load the font. We need to figure out what kind of font to use - michaelg
	{
		Logger::log("ERROR: could not load font file");
		return false;
	}

	fileDirectory = filename.substr(0, filename.find_last_of('/')); // Directory containing xml files

	text.setFont(font_main);
	text.setCharacterSize(32);
	text.setColor(sf::Color::Black);
	text.setPosition(sf::Vector2f(10.0f, 10.0f));

	// Iterate through input files and verify they are formatted correctly
	if (!Parser::verify(filename.c_str()))
	{
		return false;
	}
	
	tile = Parser::parse(filename.c_str(), boolVars, intVars, stringVars); // Get the starting tile
	if (tile.links.size() == 0 && tile.texts.size() == 0) // No links or text exists. wtf
	{
		cerr << "Error occurred while parsing start.xml" << endl;
		return false;
	}

	buildText();
	createButtons();
	buttonSelection = 0;

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
					if (b.isMouseOver(ev.mouseButton.x, ev.mouseButton.y))
					{
						loadFile(b.getLink()); // Load the next xml file
						break;
					}
				}
			if(ev.mouseButton.button == sf::Mouse::Left)
			{
				if(m->loadSelect(ev.mouseButton.x,ev.mouseButton.y))
				{
					//Add code to load a saved game.
				}
				if(m->newSelect(ev.mouseButton.x,ev.mouseButton.y))
				{
					//Add code to start a new game.
				}
				if(m->saveSelect(ev.mouseButton.x,ev.mouseButton.y))
				{
					//Add code to save current game.
				}
			}
			}
		}
		else if (ev.type == sf::Event::KeyPressed)
		{
			switch (ev.key.code)
			{
			case sf::Keyboard::O:
				cout << FileHandler::openFile(FileHandler::OpenFileMode::NEWGAME) << endl;
				break;
			case sf::Keyboard::Return:
				if (buttons.size() > 0)
				{
					loadFile(buttons[buttonSelection].getLink());
					buttonSelection = 0;
				}
				break;
			case sf::Keyboard::Up:
				if (buttonSelection < buttons.size() - 1)
				{
					int dy = buttons[buttonSelection].coords.height;
					for (auto& b : buttons)
					{
						b.setPos(b.getPos().left, b.getPos().top - dy - 10);
					}
					buttonSelection++;
				}
				break;
			case sf::Keyboard::Down:
				if (buttonSelection > 0)
				{
					int dy = buttons[buttonSelection].coords.height;
					for (auto& b : buttons)
					{
						b.setPos(b.getPos().left, b.getPos().top + dy + 10);
					}
					buttonSelection--;
				}
				break;
			}
		}
	}
}

void Game::render()
{
	window.clear(sf::Color::Cyan);

	sf::Vector2i v = sf::Mouse::getPosition(window);	//Draw menu if the mouse if over it.
	if(m->isMouseOver(v.x,v.y))
	{
		m->render(window);
		m->setTextColor(v.x,v.y);
	}
	window.draw(text);
	for (auto& button : buttons) // render the buttons
	{
		button.render(window);
	}
	sf::RectangleShape selection(sf::Vector2f(20.0f, 20.0f));
	selection.setFillColor(sf::Color::Red);
	selection.setPosition(5.0f, 250.0f);
	window.draw(selection);

	window.display();
}