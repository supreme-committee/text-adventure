#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
using namespace std;

class button
{
private:
	string text, link;
	float x,y;
	sf::RectangleShape rect;
	sf::Text t;
	sf::Font font;

public:
	button::button();
	button::button(string c, string l, float x, float y)	//Sets up a button with string t, link l, with its top left corner at (x,y).
	{                                                       //For testing, I will update this later once the tile class is finalized so a button can be instantiated with a tile as an argument.
		text = c;
		link = l;
		rect.setFillColor(sf::Color(40,40,40,100));			//Set up color, position, and size for the rectangle.
		rect.setPosition(x,y);
		rect.setSize(sf::Vector2f(100,50));
		font.loadFromFile("arial.ttf");						//Set up for text.
		t.setFont(font);
		t.setCharacterSize(16);
		t.setPosition(x+5,y+5);
		t.setString(text);
		t.setColor(sf::Color::Black);
	};

	void setTextColor(bool mouseOver)						//Will change the color of text in the box when the mouse is hovering it.
	{
		if(mouseOver)
			t.setColor(sf::Color::Red);
		else
			t.setColor(sf::Color::Black);
	}

	sf::Text getText()										//Returns sf::Text object for the window to paint.
	{
		return t;
	}
	sf::RectangleShape getRekt()							//Returns sf::RectangleShape object for window to paint.
	{
		return rect;
	}
};