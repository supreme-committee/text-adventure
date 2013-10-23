#include<SFML/Graphics.hpp>

class menu
{
public:
	menu(sf::Font&);
	void render(sf::RenderWindow&);
	bool loadSelect(int, int);
	bool saveSelect(int, int);
	bool newSelect(int, int);
	bool isMouseOver(int, int);
	void setTextColor(int, int);
private:
	sf::RectangleShape bar;
	sf::Rect<int> l,s,n,b;
	sf::Text load, save, newgame;
};