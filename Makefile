# For Windows:
# 	debug: console window will appear alongside game window
# 	release: Only the game window will appear

CSOURCE       = Main.cc Game.cc
EXENAME        = Game_Engine
SOURCEDIR    = src
SOURCEFILES = $(CSOURCE:%.cc=$(SOURCEDIR)/%.cc)

debug-mgrijalva:
	g++ -o Bin\Debug\${EXENAME} -IC:\Libraries\SFML\include ${SOURCEFILES} -LC:\Libraries\SFML\lib -lsfml-system-d -lsfml-graphics-d -lsfml-window-d
	
release-mgrijalva:
	g++ -o Bin\Release\${EXENAME} -mwindows -IC:\Libraries\SFML\include ${SOURCEFILES} -LC:\Libraries\SFML\lib -lsfml-system -lsfml-graphics -lsfml-window -lsfml-main