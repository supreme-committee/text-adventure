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
	texts.clear();

	ostringstream stringStream;
	for (string& s : tile.texts)
	{
		stringStream << s;
	}
	string textString = stringStream.str(); // All the text that needs to be displayed

	int charCount = 0; // How many characters we've iterated through
	int start = 0;     // Start of substring to break off
	int alpha = 255;   // The alpha of the line of text
	float ypos = 25.0f;
	for (unsigned int pos = 0; pos < textString.length(); pos++)
	{
		if (charCount > 70 && textString[pos] == ' ') // Line break every 80 characters
		{
			sf::Text text;
			text.setFont(font_main);
			text.setCharacterSize(16);
			text.setColor(sf::Color(255, 255, 255, alpha));
			if (alpha > 25) alpha -= 25;
			text.setString(textString.substr(start, charCount + 1));
			text.setPosition(20.0f, ypos);
			ypos += 18.0f;

			texts.push_back(text);

			start = pos + 1;
			charCount = 0;
		}
		else if (pos == textString.length() - 1) // If we're at the end just chop off the rest and break
		{
			sf::Text text;
			text.setFont(font_main);
			text.setCharacterSize(16);
			text.setColor(sf::Color(255, 255, 255, alpha));
			if (alpha > 25) alpha -= 25;
			text.setString(textString.substr(start, string::npos));
			text.setPosition(20.0f, ypos);

			texts.push_back(text);
			break;
		}
		else charCount++;
	}

	
}
void Game::createButtons()
{
	float yPos = 250.0f;
	// Create a button for each link in this tile
	for (auto& link : tile.links)
	{
		button b(link, font_main, 30.0f, yPos, muteButtons);
		buttons.push_back(b);
		yPos += b.getHeight() + 10.0f;
	}
}
void Game::scrollTextUp()
{
	if (textSelection < texts.size() - 1)
	{
		for (auto& t : texts)
		{
			t.setPosition(t.getPosition().x, t.getPosition().y - 18.0f);
		}
		textSelection++;
		int alpha = 255;
		for (int i = textSelection; i >= 0; i--)
		{
			texts[i].setColor(sf::Color(255, 255, 255, alpha));
			if (alpha > 100) alpha -= 100;
		}
		alpha = 255;
		for (int i = textSelection; i < texts.size(); i++)
		{
			texts[i].setColor(sf::Color(255, 255, 255, alpha));
			if (alpha > 25) alpha -= 25;
		}
	}
}
void Game::scrollTextDown()
{
	if (textSelection > 0)
	{
		for (auto& t : texts)
		{
			t.setPosition(t.getPosition().x, t.getPosition().y + 18.0f);
		}
		textSelection--;
		int alpha = 255;
		for (int i = textSelection; i >= 0; i--)
		{
			texts[i].setColor(sf::Color(255, 255, 255, alpha));
			if (alpha > 100) alpha -= 100;
		}
		alpha = 255;
		for (int i = textSelection; i < texts.size(); i++)
		{
			texts[i].setColor(sf::Color(255, 255, 255, alpha));
			if (alpha > 25) alpha -= 25;
		}
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
	buttons.clear();

	// Get full path to xml file
	string filePath = fileDirectory + ("/" + filename);

	// Check if xml file actually exists
	ifstream file;
	file.open(filePath, ios::in);
	if (!file.is_open())
	{
#ifdef WIN32
		thread t(&Game::showErrorMessage, this, "Cannot find file " + filename);
		t.join();
#endif
		Logger::log("ERROR: cannot find file " + filename);
		return;
	}
	file.close();

	currentFile = filename;

	tile = Parser::parse(filePath.c_str(), boolVars, intVars, stringVars);
	buildText();
	createButtons();
	if (tile.texts.size() == 1 && // if an error occurred
		tile.texts[0].find("[[ERROR]]") != string::npos)
	{
		sf::String s = this->texts[0].getString();
		s.erase(0, 10); // Erase "[[ERROR]]"
		this->texts[0].setString(s);
		for (auto& line : texts) line.setColor(sf::Color::Red);
	}

	if (tile.image.length() > 0 && texture_background.loadFromFile(".gamefiles/" + tile.image))
	{
		imageValid = true;
	}
	else imageValid = false;
	sprite_background.setTexture(texture_background);
	// For some reason setTexture doesn't set the area to display, so set it manually:
	sf::Vector2u texSize = texture_background.getSize();
	sprite_background.setTextureRect(sf::IntRect(0, 0, texSize.x, texSize.y));
}

// ================== PUBLIC FUNCTIONS ===================
Game::Game() : fileDirectory(".gamefiles")
{
	done = false;
	hideUI = false;
	tarFile = " ";
	window.create(sf::VideoMode(640, 480), "Game Engine", sf::Style::Close);
	window.setFramerateLimit(30);
	muteButtons = false;
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
	m->enableSaving(false);

	buildText();
	createButtons();
	textSelection = 0;
	buttonSelection = 0;

	if (tile.image.length() > 0 && texture_background.loadFromFile(tile.image))
	{
		imageValid = true;
	}
	else imageValid = false;
	sprite_background.setTexture(texture_background);

	return true;
}
bool Game::init(string filename)
{
	ifstream file;
	file.open(filename);
	if (!file.is_open()) 
	{
		// This init() is only called when running via command line, so user will see cerr output
		cerr << filename << " does not exist!" << endl;
		Logger::log(filename + " does not exist!");
		return false;
	}
	file.close();

	string fileExtension = filename.substr(filename.find_last_of('.'), filename.length() - 1);
	if (fileExtension != ".tar")
	{
		cerr << "ERROR: you must give me a .tar file!" << endl;
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
	m->enableSaving(true);

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
	textSelection = 0;
	buttonSelection = 0;

	if (tile.image.length() > 0 && texture_background.loadFromFile(tile.image))
	{
		imageValid = true;
	}
	else imageValid = false;
	sprite_background.setTexture(texture_background);

	return true;
}
void Game::input()
{
	sf::Event ev;
	while (window.pollEvent(ev))
	{
		if (ev.type == sf::Event::Closed) done = true;
		else if (ev.type == sf::Event::MouseButtonReleased)
		{
			if(ev.mouseButton.button == sf::Mouse::Right)	//Toggle UI on right click
			{
				if(hideUI)	//UI already hidden, restore
				{
					for (auto& b : buttons)
						b.setActiveRect(true);
					hideUI = false;
				}
				else       //UI not hidden, hide
				{
					for(auto& b : buttons)
						b.setActiveRect(false);
					hideUI = true;
				}
			}
			else if (ev.mouseButton.button == sf::Mouse::Left)
			{
				for (auto& b : buttons)
				{
					if (b.isMouseOver(ev.mouseButton.x, ev.mouseButton.y))
					{
						b.playSound();
						sf::sleep(sf::seconds(.1));	//Pause for a moment so the sound will be played.
						loadFile(b.getLink()); // Load the next xml file
						break;
					}
				}
				if(m->muteSelect(ev.mouseButton.x, ev.mouseButton.y))
				{
					for (auto& b : buttons)
					{
						b.toggleMute();
					}
					if(!muteButtons)	//Update muteButtons so that future buttons can be initialized with correct volume
						muteButtons = true;
					else muteButtons = false;
					//also mute background music/sound effects from tile once implemented
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

					string saveGametarfile = " ";
					Parser::load(saveFilename, saveGametarfile, currentFile, boolVars, intVars, stringVars);
					if (tarFile != saveGametarfile)
					{
						string error = "You must load " + saveGametarfile + " before loading this save game";
#ifdef WIN32
						thread t(&Game::showErrorMessage, this, error);
						t.join();
#else
						cerr << error << endl;
#endif
						return;
					}
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
					tarFile = gameFile.substr(gameFile.find_last_of('\\') + 1, gameFile.length() - 1); // Keep track of currently loaded tar file
					system("rmdir .gamefiles /s /q"); // Delete old xml files
#else 
                    system("rm .gamefiles/*"); // delete old files MAC specific
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
					m->enableSaving(true);
				}
				if(m->saveSelect(ev.mouseButton.x,ev.mouseButton.y)) // Saving a game
				{
					string saveFilename = FileHandler::saveFile();
					if (saveFilename.length() > 0) // If they actually chose a file
					{
						if (saveFilename.substr(saveFilename.length() - 4, saveFilename.length() - 1) != ".sav")
						{
							saveFilename += ".sav";
						}
						Parser::save(saveFilename, tarFile, currentFile, 
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
					textSelection = 0;
					buttonSelection = 0;
				}
				break;
			case sf::Keyboard::Up:
				if (buttons.size() > 0 && buttonSelection < buttons.size() - 1)
					scrollButtonsUp();
				break;
			case sf::Keyboard::Down:
				if (buttonSelection > 0)
					scrollButtonsDown();
				break;
			case sf::Keyboard::Left: // Scroll text up
				if (textSelection < texts.size() - 1)
					scrollTextUp();
				break;
			case sf::Keyboard::Right: // scroll text down
				if (textSelection > 0)
					scrollTextDown();
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
		m->setTextColor(v.x,v.y,muteButtons);
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
	{
		window.clear(sf::Color::Black);
		window.draw(sprite_background);
	}
	else
		window.clear(sf::Color::Black);

	sf::Vector2i v = sf::Mouse::getPosition(window);	//Draw menu if the mouse if over it.
	if(m->isMouseOver(v.x,v.y))
	{
		m->render(window);
	}
	if(!hideUI)
	{
		for (auto& text : texts)
		{
			window.draw(text);
		}
		for (auto& button : buttons) // render the buttons
		{
			button.render(window);
		}

		sf::CircleShape selection; // Arrow for button selection (should eventually be changed) - michaelg
		selection.setRadius(5.0f);
		selection.setFillColor(sf::Color::Red);
		selection.setPosition(5.0f, 260.0f);
		window.draw(selection);
		selection.setPosition(5.0f, 30.0f);
		window.draw(selection);
	}
	window.display();
}