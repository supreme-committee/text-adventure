#include "Game.h"
#include "parser.h"
#include "Logger.h"
#include "FileHandler.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <thread>
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

	if (tile.image.length() > 0 && texture.loadFromFile(tile.image))
	{
		imageValid = true;
	}
	else imageValid = false;
	sprite.setTexture(texture);
}

// ================== PUBLIC FUNCTIONS ===================
Game::Game() : fileDirectory(".gamefiles")
{
	done = false;
	hideUI = false;
	window.create(sf::VideoMode(640, 480), "Game Engine", sf::Style::Close);
	window.setFramerateLimit(30);
}
Game::~Game()
{
	delete m;
}
bool Game::init()
{
	if (!font_main.loadFromFile("font.ttf")) // Load the font (OpenSans-Regular)
	{
		Logger::log("ERROR: could not load font.ttf");
		return false;
	}

	m = new menu(font_main);
	m->enableLoading(false);

	text.setFont(font_main);
	text.setCharacterSize(14);
	text.setColor(sf::Color::White);
	text.setPosition(sf::Vector2f(10.0f, 25.0f));

	buildText();
	createButtons();
	buttonSelection = 0;

	if (tile.image.length() > 0 && texture.loadFromFile(tile.image))
	{
		imageValid = true;
	}
	else imageValid = false;
	sprite.setTexture(texture);

	return true;
}
bool Game::init(string filename)
{
	string fileExtension = filename.substr(filename.find_last_of('.'), filename.length() - 1);
	if (fileExtension != ".tar")
	{
		Logger::log("ERROR: you must give me a .tar file!");
		return false;
	}
	system("rmdir .gamefiles /s /q"); // Delete old xml files

	   
	system("mkdir .gamefiles");
	string command = "tar -xf '" + filename + "' -C .gamefiles";
					
	int returnCode = system(command.c_str());
#ifdef WIN32
	system("attrib +h .gamefiles");
#endif

	currentFile = filename;

	if (!font_main.loadFromFile("font.ttf")) // Load the font (OpenSans-Regular)
	{
		Logger::log("ERROR: could not load font.ttf");
		return false;
	}

	m = new menu(font_main);
	m->enableLoading(true);

	text.setFont(font_main);
	text.setCharacterSize(14);
	text.setColor(sf::Color::White);
	text.setPosition(sf::Vector2f(10.0f, 25.0f));

	// Iterate through input files and verify they are formatted correctly
	if (!Parser::verify(".gamefiles/start.xml"))
	{
		return false;
	}
	
	tile = Parser::parse(".gamefiles/start.xml", boolVars, intVars, stringVars); // Get the starting tile
	if (tile.links.size() == 0 && tile.texts.size() == 0) // No links or text exists. wtf
	{
		cerr << "Error occurred while parsing start.xml" << endl;
		return false;
	}

	buildText();
	createButtons();
	buttonSelection = 0;

	if (tile.image.length() > 0 && texture.loadFromFile(tile.image))
	{
		imageValid = true;
	}
	else imageValid = false;
	sprite.setTexture(texture);

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
		else if (ev.type == sf::Event::MouseButtonReleased)
		{
			if(ev.mouseButton.button == sf::Mouse::Right)	//Toggle UI on right click
			{
				if(hideUI)
					hideUI = false;
				else
					hideUI = true;
			}
			else if (ev.mouseButton.button == sf::Mouse::Left)
			{
				for (auto& b : buttons)
				{
					if (b.isMouseOver(ev.mouseButton.x, ev.mouseButton.y))
					{
						loadFile(b.getLink()); // Load the next xml file
						break;
					}
				}

				if(m->loadSelect(ev.mouseButton.x,ev.mouseButton.y)) // Loading a saved game
				{
					string saveFilename = FileHandler::openFile(FileHandler::OpenFileMode::SAVEGAME);
					if (saveFilename.length() == 0) return;
					if (saveFilename.substr(saveFilename.length() - 4, saveFilename.length() - 1) != ".sav")
					{
#ifdef WIN32
						thread t(&Game::showErrorMessage, this, "You must select a .sav file"); // Show an error message
						t.join();
#endif			
						return;
					}

					Parser::load(saveFilename, tarFile, currentFile, boolVars, intVars, stringVars);
					currentFile = currentFile.substr(currentFile.find_last_of('/'), currentFile.length()-1);
                    loadFile(currentFile);
				}
				if(m->newSelect(ev.mouseButton.x,ev.mouseButton.y)) // Loading a new tar file
				{
					string gameFile = FileHandler::openFile(FileHandler::OpenFileMode::NEWGAME);
					if (gameFile.length() == 0) return;
					if (gameFile.substr(gameFile.length() - 4, gameFile.length() - 1) != ".tar")
					{
#ifdef WIN32
						thread t(&Game::showErrorMessage, this, "You must select a .tar file");
						t.join();
#endif
						return;
					}
#ifdef WIN32                  
					gameFile = gameFile.substr(gameFile.find_first_of('\\'), gameFile.length() - 1);            
					system("rmdir .gamefiles /s /q"); // Delete old xml files
#else
#endif                                 
					system("mkdir .gamefiles");
					string command = "tar -xf '" + gameFile + "' -C .gamefiles";
					
					int returnCode = system(command.c_str());
#ifdef WIN32
					system("attrib +h .gamefiles");
#endif
					if (returnCode != 0)
					{
						// do something
					}

					boolVars.clear(); intVars.clear(); stringVars.clear();
					loadFile("Start.xml");
					m->enableLoading(true);
				}
				if(m->saveSelect(ev.mouseButton.x,ev.mouseButton.y)) // Saving a game
				{
					string saveFilename = FileHandler::saveFile();
					if (saveFilename != "") // If they actually chose a file
					{
						if (saveFilename.substr(saveFilename.length() - 4, saveFilename.length() - 1) != ".sav")
						{
							saveFilename += ".sav";
						}
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
	if(imageValid)
		window.draw(sprite);
	else
		window.clear(sf::Color::Black);

	sf::Vector2i v = sf::Mouse::getPosition(window);	//Draw menu if the mouse if over it.
	if(m->isMouseOver(v.x,v.y))
	{
		m->render(window);
	}
	if(!hideUI)
	{
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
	}
	window.display();
}