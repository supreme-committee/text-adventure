#include "Game.h"

Game::Game()
{
	done = false;
	window.create(sf::VideoMode(640, 480), "Game Engine", sf::Style::Close);
	window.setFramerateLimit(30);
}
Game::~Game()
{
}
void Game::init()
{
	// initialize stuff
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
	window.display();
}