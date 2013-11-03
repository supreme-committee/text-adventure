#include "button.h"

button::button(Link li, sf::Font& f,float xx, float yy)	
{
	numLines = 1;
	string str = wordWrap(li.text,40);
	int height = numLines * 20 + 10;

	link = li.filename;
	rect.setFillColor(sf::Color(40,40,40,100));			//Set up color, position, and size for the rectangle.
	rect.setPosition(xx,yy);
	rect.setSize(sf::Vector2f(250,height));
	t.setFont(f);
	t.setCharacterSize(16);
	t.setPosition(xx+10,yy+5);
	t.setString(str);
	t.setColor(sf::Color::White);
	coords = sf::Rect<int>(xx,yy,290,height);
	float h = (float)height;
	yscale = h/130;
	x = xx;
	y = yy;
}

string button::getLink()
{
	return link;
}
void button::render(sf::RenderWindow& window)
{
	texture.loadFromFile("button.png");
	sprite.setScale(1,yscale);
	sprite.setPosition(x,y);
	sprite.setTexture(texture);
	
	window.draw(sprite);
	window.draw(t);
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
		t.setColor(sf::Color::White);
		return false;
	}
}
void button::setPos(int xx, int yy)
{
	x = xx;
	y = yy;
	coords.top = y;
	coords.left = x;
	rect.setPosition(x,y);
	t.setPosition(x+10,y+5);
	sprite.setPosition(x,y);
}
sf::Rect<int> button::getPos()
{
	return coords;
}
string button::wordWrap( std::string str, size_t width = 55 )
{
	int Lines = 1;
    size_t curWidth = width;
    while( curWidth < str.length() ) {
        std::string::size_type spacePos = str.rfind( ' ', curWidth );
        if( spacePos == std::string::npos )
            spacePos = str.find( ' ', curWidth );
        if( spacePos != std::string::npos ) {
            str[ spacePos ] = '\n';
            curWidth = spacePos + width + 1;
			Lines++;
        }
    }
	numLines = Lines;
    return str;
}