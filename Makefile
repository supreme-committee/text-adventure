# For Windows:
# 	debug: console window will appear alongside game window
# 	release: Only the game window will appear

CSOURCE       = Main.cc Game.cc tile.cc parser.cc Logger.cc button.cc
EXENAME        = Game_Engine
SOURCEDIR    = src
OBJDIR = obj
SOURCEFILES = $(CSOURCE:%=$(SOURCEDIR)/%)
OBJECTS = $(CSOURCE:%.cc=$(OBJDIR)/%.o)
APPLEFRAMEWORKS = -framework OpenGL -framework SFML -framework SFML-graphics -framework SFML-window -framework SFML-system
GCC = g++ -g -O0 -Wall -Wextra -std=gnu++0x
EXECBIN = game


debug-mgrijalva:
	g++ -g -Wall -Wextra -O0 -std=gnu++0x -o Bin\Debug\${EXENAME} -IC:\Libraries\SFML\include ${SOURCEFILES} -LC:\Libraries\SFML\lib -lsfml-system-d -lsfml-graphics-d -lsfml-window-d
	
release-mgrijalva:
	g++ -O2 -std=gnu++0x -o Bin\Release\${EXENAME} -mwindows -IC:\Libraries\SFML\include ${SOURCEFILES} -LC:\Libraries\SFML\lib -lsfml-system -lsfml-graphics -lsfml-window -lsfml-main

apple: $(EXECBIN)

$(EXECBIN): $(OBJECTS)
	$(GCC) -o $(EXECBIN) $(APPLEFRAMEWORKS) $(OBJECTS)

$(OBJDIR)/%.o: $(SOURCEDIR)/%.cc
	$(GCC) -c -o $@ $<

clean:
	- rm obj/*.o
	- rm $(EXECBIN)
	- rm -r *~
