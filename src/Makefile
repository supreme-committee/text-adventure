# For Windows:
# 	debug: console window will appear alongside game window
# 	release: Only the game window will appear

CSOURCE = Main.cc Game.cc
EXENAME = Game_Engine

debug-mgrijalva:
	g++ -o Debug\${EXENAME} -IC:\Libraries\SFML\include ${CSOURCE} -LC:\Libraries\SFML\lib -lsfml-system-d -lsfml-graphics-d -lsfml-window-d
	
release-mgrijalva:
	g++ -o Release\${EXENAME} -mwindows -IC:\Libraries\SFML\include ${CSOURCE} -LC:\Libraries\SFML\lib -lsfml-system -lsfml-graphics -lsfml-window -lsfml-main