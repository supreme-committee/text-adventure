#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
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
	int numLines;
	sf::Texture texture;
	sf::Sprite sprite;
	float x,y, yscale;
	int r,g,b,alpha;
	sf::SoundBuffer sb;
	sf::Sound sound;
public:
	button();
	button(Link,sf::Font&, float, float,bool);

	void setPos(int, int);
	sf::Rect<int> getPos();						//Returns a struct with the height, width of the box, along with the top and left coordinates of the box.

	void render(sf::RenderWindow& window);
	string getLink();
	int getHeight();
	bool isMouseOver(int, int);
	sf::Rect<int> coords;
	sf::Rect<int> tempCoords;	//Holds the rect object while buttons are not on screen(rect set to 0,0,0,0).
	string wordWrap(string, size_t);
	void setAlpha(int);
	void playSound();
	void toggleMute();
	void setActiveRect(bool);
};