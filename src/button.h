#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include "tile.h"
using namespace std;

class button
{
private:
	string text, link;
	sf::RectangleShape rect;
	sf::Text t;
	sf::Font font;

public:
	button::button();
	button::button(Link,sf::Font& f, float x, float y);
	button::button(string c, string l, float x, float y, sf::Font& f);	//Sets up a button with string t, link l, with its top left corner at (x,y).
	void render(sf::RenderWindow& window);
	string getLink();
	int getHeight();
	bool isMouseOver(int, int);
	sf::Rect<int> coords;
};