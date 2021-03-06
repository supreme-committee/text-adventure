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

	while (textString[0] == '\n' || textString[0] == ' ') // Chop off line breaks and empty space
	{
		textString.replace(0, 1, ""); 
	}
	while (textString.find("\r") != string::npos) // Replace return characters
	{
		textString.replace(textString.find("\r"), 1, " ");
	}

	int charCount = 0; // How many characters we've iterated through
	int start = 0;     // Start of substring to break off
	int alpha = 255;   // The alpha of the line of text
	float ypos = 25.0f;
	for (unsigned int pos = 0; pos < textString.length(); pos++)
	{
		if (textString[pos] == '\n' || // Line break when '\n' is found
			(charCount > 110 && textString[pos] == ' ')) // Or just line break every 110 characters
		{
			sf::Text text;
			text.setFont(font_main);
			text.setCharacterSize(16);
			text.setColor(sf::Color(255, 255, 255, alpha));
			if (alpha > 25) alpha -= 25;
			else alpha = 0;
			string text_string = textString.substr(start, charCount + 1);
			while (text_string[0] == ' ') text_string.replace(0, 1, ""); // Trim white space
			text.setString(text_string);
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
	float yPos = 300.0f;
	// Create a button for each link in this tile
	for (auto& link : tile.links)
	{
		button b(link, font_main, 30.0f, yPos, muteButtons);
		float alpha = (yPos / 300.0f) * 255.0f;
		if (alpha > 255.0f) alpha = 255.0f;
		b.setAlpha(alpha);
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
			texts[i].setColor(sf::Color::Transparent);
		}
		alpha = 255;
		for (unsigned int i = textSelection; i < texts.size(); i++)
		{
			texts[i].setColor(sf::Color(255, 255, 255, alpha));
			if (alpha > 25) alpha -= 25;
			else alpha = 0;
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
			texts[i].setColor(sf::Color::Transparent);
		}
		alpha = 255;
		for (unsigned int i = textSelection; i < texts.size(); i++)
		{
			texts[i].setColor(sf::Color(255, 255, 255, alpha));
			if (alpha > 25) alpha -= 25;
			else alpha = 0;
		}
	}
}
void Game::scrollButtonsUp()
{
	if (buttonSelection < buttons.size() - 1)
	{
		while (buttons[buttonSelection + 1].getPos().top > 300)
		{
			for (auto& b : buttons)
			{
				b.setPos(b.getPos().left, b.getPos().top - 1);
				if (b.getPos().top < 212.0f) b.setAlpha(0);
				else if (b.getPos().top < 300.0f) b.setAlpha(100);
			}
			this_thread::sleep_for(chrono::milliseconds(2));
		}
		buttonSelection++;
	}
	buttonsAnimating = false;
}
void Game::scrollButtonsDown()
{
	if (buttonSelection > 0)
	{
		while (buttons[buttonSelection - 1].getPos().top < 300)
		{
			for (auto& b : buttons)
			{
				b.setPos(b.getPos().left, b.getPos().top + 1);
				if (b.getPos().top >= 300) b.setAlpha(255);
				else if (b.getPos().top > 212) b.setAlpha(100);
			}
			this_thread::sleep_for(chrono::milliseconds(2));
		}
		buttonSelection--;
	}
	buttonsAnimating = false;
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
    
    Parser::verify((".gamefiles/" + filename).c_str());
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

	
	if (tile.image.length() > 1)
	{
		string supportedImageTypes[] = {"bmp", "png", "tga", "jpg", "gif", "psd", "hdr", "pic"};
		string extension = tile.image.substr(tile.image.find_last_of(".") + 1, string::npos);
		bool valid = false;
		for (auto& s : supportedImageTypes)
		{
			if (extension == s) {
				valid = true;
				break;
			}
		}
		if (valid && texture_background.loadFromFile(".gamefiles/" + tile.image))
		{
			imageValid = true;
			rescaleImage();
		}
		else if (!valid) showErrorMessage("Invalid image format: " + tile.image);
	}
	else imageValid = false;
	sprite_background.setTexture(texture_background);
	// For some reason setTexture doesn't set the area to display, so set it manually:
	sf::Vector2u texSize = texture_background.getSize();
	sprite_background.setTextureRect(sf::IntRect(0, 0, texSize.x, texSize.y));

	sound->resetBuffer();	//Reset buffer to be ready to load a new one.
	if(tile.sfx.length() > 0)
	{
		string supportedTypes[] = {
		"ogg", "wav", "flac", "aiff", "au", 
		"raw", "paf", "svx", "nist", "voc", 
		"ircam", "w64", "mat4", "mat5", "pvf", 
		"htk", "sds", "avr", "sd2", "caf", "wve", "mpc2k", "rf64"};
		string ext = tile.sfx.substr(tile.sfx.find_last_of(".") + 1, string::npos);
		bool valid = false;
		for (auto s : supportedTypes)
		{
			if (ext == s) {
				valid = true;
				break;
			}
		}
		if(valid && !soundbuffer.loadFromFile(".gamefiles/" + tile.sfx))
		{
			Logger::log("Could not open sound file .gamefiles/" + tile.sfx);
			sound->setVolume(0);	//If no sound is loaded, set the volume to 0 just to be sure it doesn't play anything
		}
		else
		{
			if (!valid) showErrorMessage("Invalid sound format: " + tile.sfx);
			sound->setBuffer(soundbuffer);
			sound->setVolume(100);
			if(!muteButtons)
				sound->play();
		}
	}
	if(tile.bgm != "")
		loadMusic(".gamefiles/" + tile.bgm);

	hideUI = false;
}
void Game::setupNewGame(string tarFile)
{
#ifdef WIN32
	string command = "rmdir .gamefiles /s /q & mkdir .gamefiles && attrib +h .gamefiles && tar -xf '" + tarFile + "' -C .gamefiles";
	system(command.c_str());
#else
    system("mkdir .gamefiles");
	system("rm .gamefiles/*"); // delete old files MAC specific
	system(("tar -xf " + tarFile + " -C .gamefiles").c_str());
#endif

	boolVars.clear(); intVars.clear(); stringVars.clear();
	loadFile("Start.xml");

	m->enableLoading(true);
	m->enableSaving(true);

	rect_overlay.setSize(sf::Vector2f(1024.0f, 576.0f));
	rect_overlay.setFillColor(sf::Color::Transparent);
}

// ================== PUBLIC FUNCTIONS ===================
Game::Game() : fileDirectory(".gamefiles")
{
	done = false;
	hideUI = false;
	tarFile = " ";
	window.create(sf::VideoMode(1024, 576), "Fork in the Read", sf::Style::Close);
	window.setFramerateLimit(30);
	muteButtons = false;
	rect_textBackground.setPosition(4,22);
	rect_textBackground.setSize(sf::Vector2f(1014,190));
	rect_textBackground.setFillColor(sf::Color(0,0,0,150));
	tran_state = STATIC;
	buttonsAnimating = false;
}
Game::~Game()
{
	delete m;
	delete sound;
	delete bgm;
}
void Game::init(string tarFile) // tarFile is " " by default
{
	if (!font_main.loadFromFile("content/font.ttf")) { // Load the font (OpenSans-Regular)
		Logger::log("ERROR: could not load font.ttf");
		showErrorMessage("ERROR: could not load font.ttf");
	}

	if (!tex_arrow.loadFromFile("content/arrow.png")) {
		Logger::log("ERROR: could not load arrow.png");
		showErrorMessage("ERROR: could not load arrow.png");
	}
	this->spr_arrow.setTexture(tex_arrow);

	if (texture_background.loadFromFile("content/logo.png")) {
		imageValid = true;
		rescaleImage();
		sprite_background.setTexture(texture_background);
	}
	bgm = new sf::Music();
	sound = new sf::Sound();
	m = new menu(font_main);
	m->enableLoading(false);
	m->enableSaving(false);

	if (tarFile != " ") // If user provided a tar file
	{
		ifstream file(tarFile, ios::in);
		if (!file.is_open()) {
			Logger::log("ERROR: Could not find " + tarFile);
			showErrorMessage("ERROR: Could not find " + tarFile);
			return;
		}
		else file.close();

		string fileExt = tarFile.substr(tarFile.find_last_of("."), string::npos);
		if (fileExt != ".tar") {
			Logger::log("ERROR: invalid file type: " + fileExt);
			showErrorMessage("ERROR: invalid file type: " + fileExt);
		}

		setupNewGame(tarFile); // Extract the tar, load start.xml
		this->currentFile = ".gamefiles/Start.xml";

		// Verify the content of the start.xml file
		if (!Parser::verify(currentFile.c_str())) {
			showErrorMessage("ERROR: file failed verification: " + currentFile);
		}

		tile = Parser::parse(currentFile.c_str(), boolVars, intVars, stringVars); // Parse the first file
		if (tile.texts.size() == 1 && // if an error occurred
			tile.texts[0].find("[[ERROR]]") != string::npos) {
			sf::String s = this->texts[0].getString();
			s.erase(0, 10); // Erase "[[ERROR]]"
			this->texts[0].setString(s);
			for (auto& line : texts) line.setColor(sf::Color::Red);
			m->enableSaving(false);
			m->enableLoading(false);
		}
		else {
			buildText();
			createButtons();
			m->enableLoading(true); // Enable saving and loading buttons
			m->enableSaving(true);
		}

		// Load and scale the image for this tile
		if (tile.image.length() > 1 && texture_background.loadFromFile(tile.image)) {
			this->imageValid = true;
			rescaleImage();
		}
		else this->imageValid = false;
		sprite_background.setTexture(texture_background);

		sound->resetBuffer(); // Load and set up the sound effect for this tile
		if(tile.sfx.length() > 1 && !soundbuffer.loadFromFile(".gamefiles/" + tile.sfx)) {
			Logger::log("Could not open sound file .gamefiles/" + tile.sfx);
			showErrorMessage("ERROR: Could not open sound file: " + tile.sfx);
			sound->setVolume(0);	//If no sound is loaded, set the volume to 0 just to be sure it doesn't play anything
		}
		else sound->setVolume(100);
		sound->setBuffer(soundbuffer);

		if (!muteButtons) sound->play();
		if (tile.bgm.length() > 1) loadMusic(".gamefiles/" + tile.bgm);
	}

	textSelection = 0;
	buttonSelection = 0;
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
				else if (imageValid) // UI not hidden, hide if there is a background image
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
						this->tran_state = FADING_OUT; // Fade everything out
						this->currentFile = b.getLink(); // Prepare next xml file for loading
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
					{
						bgm->setVolume(0);
						muteButtons = true;
						sound->setVolume(0);
					}
					else
					{
						bgm->setVolume(100);
						muteButtons = false;
						sound->setVolume(100);
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
#endif
					tarFile = gameFile.substr(gameFile.find_last_of('\\') + 1, gameFile.length() - 1); // Keep track of currently loaded tar file
					sound->stop();
					sound->resetBuffer();
					delete sound;
					sound = new sf::Sound();

					bgm->stop();
					delete bgm;
					bgm = new sf::Music();
					currentMusic = "";	//reset stored name of current music file being played in case the new game will play the same file that was last played
					setupNewGame(gameFile);                             
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
			case sf::Keyboard::Return:
				if (buttons.size() > 0)
				{
					buttons[buttonSelection].playSound();
					sf::sleep(sf::seconds(.1));	//Pause for a moment so the sound will be played.
					this->tran_state = FADING_OUT; // Fade everything out
					this->currentFile = buttons[buttonSelection].getLink(); // Prepare next xml file for loading
					buttonSelection = 0;
					textSelection = 0;
				}
				break;
			case sf::Keyboard::Up: // Scroll buttons up
				while (buttonsAnimating) this_thread::sleep_for(chrono::milliseconds(1)); // Sleep until prev. animation is done
				if (buttons.size() > 0 && buttonSelection < buttons.size() - 1) {
					buttonsAnimating = true;
					thread t(&Game::scrollButtonsUp, this);
					t.detach();
				}
				break;
			case sf::Keyboard::Down: // Scroll buttons down
				while (buttonsAnimating) this_thread::sleep_for(chrono::milliseconds(1));
				if (buttonSelection > 0) {
					buttonsAnimating = true;
					thread t(&Game::scrollButtonsDown, this);
					t.detach();
				}
				break;
			case sf::Keyboard::Right: // Scroll text down
				if (texts.size() > 9 && textSelection < texts.size() - 1)
					scrollTextUp();
				break;
			case sf::Keyboard::Left: // scroll text up
				if (texts.size() > 9 && textSelection > 0)
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

	if (this->tran_state == FADING_OUT) // Increase alpha of overlay
	{
		int alpha = (int)rect_overlay.getFillColor().a + 15;
		if (alpha >= 255) { // Done fading out, so load next file and fade in
			alpha = 255; 
			rect_overlay.setFillColor(sf::Color(0, 0, 0, alpha));
			this->tran_state = FADING_IN; 
		}
		rect_overlay.setFillColor(sf::Color(0, 0, 0, alpha));
	}
	else if (this->tran_state == FADING_IN) // Decrease alpha of overlay
	{
		int alpha = (int)rect_overlay.getFillColor().a;
		if (alpha >= 255) loadFile(this->currentFile); // Only load next file when overlay is 100% opaque
		alpha -= 15;
		if (alpha <= 0) { alpha = 0; this->tran_state = STATIC; }
		rect_overlay.setFillColor(sf::Color(0, 0, 0, alpha));
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
		if (texts.size() > 0) window.draw(rect_textBackground); // Background for text
		for (auto& text : texts)
		{
			window.draw(text);
		}
		for (auto& button : buttons) // render the buttons
		{
			button.render(window);
		}

		if (buttons.size() > 0) {
			this->spr_arrow.setPosition(0, 315); // Draw the arrow
			window.draw(spr_arrow);
		}
		if (texts.size() > 0) {
			this->spr_arrow.setPosition(-9, 30);
			window.draw(spr_arrow);
		}
	}
	window.draw(rect_overlay); // Draw the overlay over everything else
	window.display();
}
void Game::rescaleImage()
{
	sf::Vector2u dimension_img = texture_background.getSize();
	sf::Vector2u dimension_window = window.getSize();
	if(dimension_img.x != 1024 || dimension_img.y != 576)	//image is not 1024X576, need to rescale
	{
		float aspect_img = (float)dimension_img.x / (float)dimension_img.y;	//get aspect ratio of image

		float scaleX = (float)dimension_window.x / (float)dimension_img.x;	//get ratio of window's width to image's width
		float scaleY = (float)dimension_window.y / (float)dimension_img.y;	//get ratio of window's height to image's height

		if(aspect_img == (16.0/9.0))	//image is too large or too small, but has 16:9 ratio
		{
			sprite_background.setScale(scaleX,scaleY);
		}
		else if(aspect_img > (16.0/9.0))	//image has largest width, so scale x and y by width
		{
			sprite_background.setScale(scaleX,scaleX);	//set scale
			float differenceY = 576 - (scaleX * dimension_img.y);	//get difference between window height and scaled height
			if(differenceY < 0)
				differenceY *= -1;
			sprite_background.setPosition(0,differenceY/2.0);	//Position image to make letterbox
		}
		else if(aspect_img < (16.0/9.0))	//image has largest height, so scale x and y by height
		{
			sprite_background.setScale(scaleY,scaleY);
			float differenceX = 1024 - (scaleY * dimension_img.x);
			if(differenceX < 0)
				differenceX *= -1;
			sprite_background.setPosition(differenceX/2.0,0);
		}
	}
	else
	{
		sprite_background.setScale(1.0,1.0);
		sprite_background.setPosition(0,0);
	}
}
void Game::loadMusic(string filename)
{
	string supportedTypes[] = {
		"ogg", "wav", "flac", "aiff", "au", 
		"raw", "paf", "svx", "nist", "voc", 
		"ircam", "w64", "mat4", "mat5", "pvf", 
		"htk", "sds", "avr", "sd2", "caf", "wve", "mpc2k", "rf64"};
	string ext = filename.substr(filename.find_last_of(".") + 1, string::npos);
	bool good = false;
	for (auto s : supportedTypes)
	{
		if (ext == s) {
			good = true;
			break;
		}
	}
	if (!good)
	{
#ifdef WIN32
		showErrorMessage("Invalid music format: " + filename);
#else
		cerr << "Invalid file format: " << filename << endl;
#endif
		return;
	}

	if(filename != "")	//don't bother trying to load music on a tile with none specified
	{
		if(filename != currentMusic)	//make sure that the music file from the tile is not already being played.
		{
			if(!bgm->openFromFile(filename))
			{
				Logger::log("Could not open music file" + filename + ".");
#ifdef WIN32
				thread t(&Game::showErrorMessage, this, "Cannot find file: " + filename);
				t.join();
#endif
			}
			else
			{
				bgm->setLoop(true);
				bgm->play();
				currentMusic = filename;
			}
		}
	}

}