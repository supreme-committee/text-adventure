#include "button.h"
#include "Logger.h"

button::button(Link li, sf::Font& f,float xx, float yy,bool mute)	
{
	numLines = 1;
	string str = wordWrap(li.text,70);
	int height = numLines * 20 + 10;

	link = li.filename;
	rect.setFillColor(sf::Color(40,40,40,100));			//Set up color, position, and size for the rectangle.
	rect.setPosition(xx,yy);
	rect.setSize(sf::Vector2f(250,height));
	t.setFont(f);
	t.setCharacterSize(16);
	t.setPosition(xx+20,yy+5);
	t.setString(str);
	t.setColor(sf::Color::White);
	coords = sf::Rect<int>(xx,yy,500,height);
	tempCoords = coords;
	float h = (float)height;
	yscale = h/120;
	x = xx;
	y = yy;
	r = 0;
	g = 0;
	b = 255;
	alpha = 255;

	if (!texture.loadFromFile("content/button6.png"))
	{
		Logger::log("Failed to load button.png It could not be found!");
	}
	if(mute)
		sound.setVolume(0);
	else sound.setVolume(100);
}
string button::getLink()
{
	return link;
}
void button::render(sf::RenderWindow& window)
{
    //float xscale = (numLines > 1) ? 2.0 : ((2.0/60) * static_cast<string>(t.getString()).size());
    float xscale = 2.0;
    if(numLines == 1)
    {
		xscale = (2.0/60) * t.getString().getSize();
    }
	sprite.setScale(xscale,yscale);
    if(numLines > 1)
    {        
	    sprite.setPosition(x,y);
    }
    else 
    {
        int offset = 4/xscale;
        sprite.setPosition(x+offset, y);
    }

	sprite.setTexture(texture);
	sprite.setColor(sf::Color(r,g,b,alpha));
	
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
		r = 255;
		g = 0;
		b= 0;
		return true;
	}
	else
	{
		r = 0;
		g = 0;
		b= 255;
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
	t.setPosition(x+20,y+5);
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
void button::setAlpha(int a)
{
	alpha = a;
	t.setColor(sf::Color(255, 255, 255, a));
}
void button::playSound()
{
	if(!sb.loadFromFile("content/button.wav"))
	{
		Logger::log("failed to load button.wav: It could not be found.");
	}
	sound.setBuffer(sb);
	sound.play();
}
void button::toggleMute()
{
	if(sound.getVolume() == 0)
		sound.setVolume(100);
	else sound.setVolume(0);
}
void button::setActiveRect(bool active)
{
	if(!active)
		coords = sf::Rect<int>(0,0,0,0);
	else coords = tempCoords;
}