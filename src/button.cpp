#include "button.h"

button::button(Link li, sf::Font& f,float x, float y)	//
{
	link = li.filename;
	rect.setFillColor(sf::Color(40,40,40,100));			//Set up color, position, and size for the rectangle.
	rect.setPosition(x,y);
	rect.setSize(sf::Vector2f(100,50));
	t.setFont(f);
	t.setCharacterSize(16);
	t.setPosition(x+5,y+5);
	t.setString(li.text);
	t.setColor(sf::Color::Black);
	coords = sf::Rect<int>(x,y,100,50);
}
button::button(string c, string l, float x, float y, sf::Font& f)	//Sets up a button with string t, link l, with its top left corner at (x,y).
{                                                       //For testing, I will update this later once the tile class is finalized so a button can be instantiated with a tile as an argument.
	text = c;
	link = l;
	font = f;
	rect.setFillColor(sf::Color(40,40,40,100));			//Set up color, position, and size for the rectangle.
	rect.setPosition(x,y);
	rect.setSize(sf::Vector2f(100,50));
	t.setFont(font);
	t.setCharacterSize(16);
	t.setPosition(x+5,y+5);
	t.setString(text);
	t.setColor(sf::Color::Black);
	coords = sf::Rect<int>(x,y,100,50);
}
string button::getLink()
{
	return link;
}
void button::render(sf::RenderWindow& window)
{
	window.draw(t);
	window.draw(rect);
}
int button::getHeight()
{
	return coords.height;
}

bool button::isMouseOver(int x, int y)
{
	if(coords.contains(sf::Vector2i(x,y)))
	{
		t.setColor(sf::Color::Red);
		return true;
	}
	else
	{
		t.setColor(sf::Color::Black);
		return false;
	}
}