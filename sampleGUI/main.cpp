#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "button.h"
using namespace std;

string wordWrap( std::string str, size_t width = 55 )
//Takes a string and makes it word wrap to fit inside the text box.
{
    size_t curWidth = width;
    while( curWidth < str.length() ) {
        std::string::size_type spacePos = str.rfind( ' ', curWidth );
        if( spacePos == std::string::npos )
            spacePos = str.find( ' ', curWidth );
        if( spacePos != std::string::npos ) {
            str[ spacePos ] = '\n';
            curWidth = spacePos + width + 1;
        }
    }
    return str;
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Sample GUI", sf::Style::Close);
	window.setFramerateLimit(60);

	sf::Event ev;
	sf::RectangleShape rekt(sf::Vector2f(790,150));
	rekt.setFillColor(sf::Color(40,40,40,100));
	rekt.setPosition(5,445);

	sf::Font font;
	if(!font.loadFromFile("arial.ttf"))
	{
		cout << "Couldn't load font file" << endl;
	}

	//Sample string for main text box to show word wrapping. A single word can't be more than 55 chars long.
	string str = "iubagivuaedbfgiub eraiguberaigubesdfrgset rgsterghrtseqqq qqqwwweeeeeeehsrwth rthhtafbwiyerufghb aiyfrdvbgaiweygbfriyuwgb aiwyufbgaiwyebvfiuywa awyugbfiawyubefiuywbef aiwuebfiuwabefiuwab awieuhgiaewrughbaer";
	str = wordWrap(str,65);		//Cuts off a line at 65 characters, can be changed if needed.

	sf::Text text;
	text.setFont(font);
	text.setColor(sf::Color::Black);
	text.setString(str);
	text.setCharacterSize(22);
	text.setPosition(10,455);

	button c1("choice1", "link1",250,200);	//Make sample buttons
	button c2("choice2", "link2",450,200);

	while(window.isOpen())
	{
		window.clear(sf::Color::Green);
		window.draw(text);
		window.draw(rekt);
		window.draw(c1.getRekt());
		window.draw(c1.getText());
		window.draw(c2.getRekt());
		window.draw(c2.getText());

		sf::Vector2i vect = sf::Mouse::getPosition(window);		//Checks the mouse position and will change the color of the button's text is the mouse is hovering over it.
		if(vect.x >= 250 && vect.x <= 350 && vect.y >= 200 && vect.y <= 250)
			c1.setTextColor(true);
		else
			c1.setTextColor(false);

		if(vect.x >= 450 && vect.x <= 550 && vect.y >= 200 && vect.y <= 250)
			c2.setTextColor(true);
		else
			c2.setTextColor(false);

		if(sf::Mouse::isButtonPressed(sf::Mouse::Left))	//Checks where the mouse is when it is clicked and will(eventually) display the new text/buttons from the link.
		{
		sf::Vector2i vect = sf::Mouse::getPosition(window);
		if(vect.x >= 250 && vect.x <= 350 && vect.y >= 200 && vect.y <= 250)
			str = "you chose option 1";
		if(vect.x >= 450 && vect.x <= 550 && vect.y >= 200 && vect.y <= 250)
			str = "you chose option 2";
		text.setString(str);

		//To do: get tile that is linked and update the text box and buttons
		}


		window.display();

		while(window.pollEvent(ev))
		{
			if(ev.type == sf::Event::Closed)
				window.close();
		}
	}
	return 0;
}