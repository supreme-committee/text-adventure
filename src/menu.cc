#include "menu.h"

menu::menu(sf::Font& f)
{
	load.setFont(f);
	load.setColor(sf::Color::Black);
	load.setCharacterSize(16);
	load.setPosition(160,0);
	load.setString("Load");

	save.setFont(f);
	save.setColor(sf::Color::Black);
	save.setCharacterSize(16);
	save.setPosition(100,0);
	save.setString("Save");


	newgame.setFont(f);
	newgame.setColor(sf::Color::Black);
	newgame.setCharacterSize(16);
	newgame.setPosition(0,0);
	newgame.setString("New Game");

	bar.setFillColor(sf::Color(205,192,176,255));
	bar.setSize(sf::Vector2f(640,22));
	bar.setPosition(0,0);

	l.height = 22;
	l.width = 45;
	l.top = 0;
	l.left = 160;

	s.height = 22;
	s.width = 45;
	s.top = 0;
	s.left = 100;

	n.height = 22;
	n.width = 95;
	n.top = 0;
	n.left = 0;

	b.height = 22;
	b.width = 640;
	b.left = 0;
	b.top = 0;
}
void menu::render(sf::RenderWindow& win)
{
	win.draw(bar);
	win.draw(load);
	win.draw(save);
	win.draw(newgame);
}
bool menu::isMouseOver(int x, int y)
{
	if(b.contains(sf::Vector2i(x,y)))
		return true;
	else return false;
}
bool menu::loadSelect(int x, int y)
{
	if(l.contains(sf::Vector2i(x,y)))
		return true;
	else return false;
}
bool menu::saveSelect(int x, int y)
{
	if(s.contains(sf::Vector2i(x,y)))
		return true;
	else return false;
}
bool menu::newSelect(int x, int y)
{
	if(n.contains(sf::Vector2i(x,y)))
		return true;
	else return false;
}
void menu::setTextColor(int x, int y)
{
	if(n.contains(sf::Vector2i(x,y)))
		newgame.setColor(sf::Color::Red);
	else newgame.setColor(sf::Color::Black);

	if(s.contains(sf::Vector2i(x,y)))
		save.setColor(sf::Color::Red);
	else save.setColor(sf::Color::Black);

	if(l.contains(sf::Vector2i(x,y)))
		load.setColor(sf::Color::Red);
	else 
	{
		if (l.height == 0) load.setColor(sf::Color(0, 0, 0, 50)); // If loading is disabled
		else load.setColor(sf::Color::Black);
	}
}
void menu::setActive(bool active)	//Disables the save button if given false, enables it if true
{
	if(!active)
	{
		s.height = 0;
		s.left = 0;
		s.top = 0;
		s.width = 0;
		save.setColor(sf::Color(185,172,156,255));
	}
	else
	{
		s.height = 22;
		s.width = 45;
		s.top = 0;
		s.left = 100;
		save.setColor(sf::Color::Black);
	}
}
void menu::enableLoading(bool enabled)
{
	if (!enabled)
	{
		l.height = 0;
		l.left = 0;
		l.top = 0;
		l.width = 0;
		load.setColor(sf::Color(0, 0, 0, 50));
	}
	else
	{
		l.height = 22;
		l.width = 45;
		l.top = 0;
		l.left = 160;
		load.setColor(sf::Color::Black);
	}
}