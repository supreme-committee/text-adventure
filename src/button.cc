#include "button.h"

button::button(Link li, sf::Font& f,float x, float y)	
{
	numLines = 1;
	string str = wordWrap(li.text,22);
	int height = numLines * 20 + 10;

	link = li.filename;
	rect.setFillColor(sf::Color(40,40,40,100));			//Set up color, position, and size for the rectangle.
	rect.setPosition(x,y);
	rect.setSize(sf::Vector2f(150,height));
	t.setFont(f);
	t.setCharacterSize(16);
	t.setPosition(x+5,y+5);
	t.setString(str);
	t.setColor(sf::Color::White);
	coords = sf::Rect<int>(x,y,150,height);
}
button::button(string c, string l, float x, float y, sf::Font& f)	//Sets up a button with string t, link l, with its top left corner at (x,y).
{           
                                            //For testing, I will update this later once the tile class is finalized so a button can be instantiated with a tile as an argument.
		numLines = 1;
		string str = wordWrap(c,22);
		cout << numLines << endl;
		int height = numLines * 20 + 10;
		text = str;
		link = l;
		font = f;
		rect.setFillColor(sf::Color(40,40,40,100));			//Set up color, position, and size for the rectangle.
		rect.setPosition(x,y);
		rect.setSize(sf::Vector2f(150,height));
		t.setFont(font);
		t.setCharacterSize(16);
		t.setPosition(x+5,y+5);
		t.setString(text);
		t.setColor(sf::Color::White);
		coords = sf::Rect<int>(x,y,150,height);

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
		t.setColor(sf::Color::White);
		return false;
	}
}
void button::setPos(int x, int y)
{
	coords.top = y;
	coords.left = x;
	rect.setPosition(x,y);
	t.setPosition(x+5,y+5);
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
			cout << "adding newline" << endl;
            curWidth = spacePos + width + 1;
			Lines++;
        }
    }
	numLines = Lines;
    return str;
}