#pragma once
#include<SFML/Graphics.hpp>

class menu
{
public:
	menu(sf::Font&);
	void render(sf::RenderWindow&);
	bool loadSelect(int, int);
	bool saveSelect(int, int);
	bool newSelect(int, int);
	bool muteSelect(int,int);
	bool isMouseOver(int, int);
	void setTextColor(int, int, bool);
	void enableSaving(bool);  // Eanble/disable saving button
	void enableLoading(bool); // Enable/disable loading button

private:
	sf::RectangleShape bar;
	sf::Rect<int> l,s,n,b,m;
	sf::Text load, save, newgame, mute;
};