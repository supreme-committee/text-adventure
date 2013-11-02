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
	for (unsigned int pos = 0; pos < textString.length(); pos++)
	{
		if (charCount > 80 && textString[pos] == ' ') // Line break every 80 characters
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
void Game::scrollButtonsUp()
{
	if (buttonSelection < buttons.size() - 1)
	{
		int dy = buttons[buttonSelection].coords.height / 2
			+ buttons[buttonSelection + 1].coords.height / 2 + 10;
		for (auto& b : buttons)
		{
			b.setPos(b.getPos().left, b.getPos().top - dy);
		}
		buttonSelection++;
	}
}
void Game::scrollButtonsDown()
{
	if (buttonSelection > 0)
	{
		int dy = buttons[buttonSelection].coords.height / 2
			+ buttons[buttonSelection - 1].coords.height / 2 + 10;
		for (auto& b : buttons)
		{
			b.setPos(b.getPos().left, b.getPos().top + dy);
		}
		buttonSelection--;
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
		text.setColor(sf::Color::Red);
		text.setCharacterSize(32);
		text.setString("ERROR: cannot find file:\n" + filename);
		Logger::log("ERROR: cannot find file " + filename);
		return;
	}
	file.close();

	currentFile = filename;

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
	if (!font_main.loadFromFile("font.ttf")) // Load the font (OpenSans-Regular)
	{
		Logger::log("ERROR: could not load font.ttf");
		return false;
	}

	m = new menu(font_main);

	fileDirectory = filename.substr(0, filename.find_last_of('/')); // Directory containing xml files

	text.setFont(font_main);
	text.setCharacterSize(14);
	text.setColor(sf::Color::White);
	text.setPosition(sf::Vector2f(10.0f, 25.0f));

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
		else if (ev.type == sf::Event::MouseWheelMoved)
		{
			if (ev.mouseWheel.delta > 0) // Wheel up
			{
				if (buttonSelection < buttons.size() - 1)
				{
					scrollButtonsUp();
				}
			}
			else if (ev.mouseWheel.delta < 0) // Wheel down
			{
				if (buttonSelection > 0)
				{
					scrollButtonsDown();
				}
			}
		}
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

				if(m->loadSelect(ev.mouseButton.x,ev.mouseButton.y))
				{
					string saveFilename = FileHandler::openFile(FileHandler::OpenFileMode::SAVEGAME);
					// Load the given .save file...
				}
				if(m->newSelect(ev.mouseButton.x,ev.mouseButton.y))
				{
					string gameFile = FileHandler::openFile(FileHandler::OpenFileMode::NEWGAME);
					// Load the new .tar file...
				}
				if(m->saveSelect(ev.mouseButton.x,ev.mouseButton.y))
				{
					string saveFilename = FileHandler::saveFile();
					if (saveFilename != "") // If they actually chose a file
					{
						saveFilename += ".sav";
						Parser::save(saveFilename, "tarfile.tar", currentFile, 
							boolVars, intVars, stringVars);
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
				if (buttons.size() > 0 && buttonSelection < buttons.size() - 1)
				{
					scrollButtonsUp();
				}
				break;
			case sf::Keyboard::Down:
				if (buttonSelection > 0)
				{
					scrollButtonsDown();
				}
				break;
			default:
				break;
			}
		}
	}
}

void Game::update()
{
	// Update the menu bar
	sf::Vector2i v = sf::Mouse::getPosition(window);	//Draw menu if the mouse if over it.
	if(m->isMouseOver(v.x,v.y))
	{
		m->setTextColor(v.x,v.y);
	}

	// Update buttons
	for (auto& button : buttons)
	{
		button.isMouseOver(v.x,v.y);
	}
}

void Game::render()
{
	window.clear(sf::Color::Black); // Background color

	sf::Vector2i v = sf::Mouse::getPosition(window);	//Draw menu if the mouse if over it.
	if(m->isMouseOver(v.x,v.y))
	{
		m->render(window);
	}
	window.draw(text);
	for (auto& button : buttons) // render the buttons
	{
		button.render(window);
	}

	sf::CircleShape selection; // Arrow for button selection (should eventually be changed) - michaelg
	selection.setRadius(10.0f);
	selection.setFillColor(sf::Color::White);
	selection.setPosition(5.0f, 255.0f);
	window.draw(selection);

	window.display();
}