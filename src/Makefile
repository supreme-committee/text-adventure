# For Windows:
# 	debug: console window will appear alongside game window
# 	release: Only the game window will appear

CSOURCE = Main.cpp Game.cpp
EXENAME = Game_Engine

debug-mgrijalva:
	g++ -o ${EXENAME} -IC:\Libraries\SFML\include ${CSOURCE} -LC:\Libraries\SFML\lib -lsfml-system -lsfml-graphics -lsfml-window
	
release-mgrijalva:
	g++ -o ${EXENAME} -mwindows -IC:\Libraries\SFML\include ${CSOURCE} -LC:\Libraries\SFML\lib -lsfml-system -lsfml-graphics -lsfml-window -lsfml-main