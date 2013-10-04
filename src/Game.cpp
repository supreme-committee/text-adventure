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
	}
}
void Game::render()
{
	window.clear(sf::Color::Black);
	window.display();
}