# For Windows:
# 	debug: console window will appear alongside game window
# 	release: Only the game window will appear

CSOURCE       = Main.cc Game.cc tile.cc parser.cc Logger.cc button.cc FileHandler.cc menu.cc
EXENAME        = Game_Engine
SOURCEDIR    = src
OBJDIR = obj
SOURCEFILES = $(CSOURCE:%=$(SOURCEDIR)/%)
OBJECTS = $(CSOURCE:%.cc=$(OBJDIR)/%.o)
APPLEFRAMEWORKS = -framework OpenGL -framework SFML -framework SFML-graphics -framework SFML-window -framework SFML-system -framework SFML-audio
GCC = g++ -g -O0 -Wall -Wextra -std=gnu++0x
EXECBIN = game


apple: $(EXECBIN)
	@echo "\n=====================\n   Compile Success\n=====================\n"


$(EXECBIN): $(OBJECTS)
	$(GCC) -o $(EXECBIN) $(APPLEFRAMEWORKS) $(OBJECTS)


$(OBJDIR)/%.o: $(SOURCEDIR)/%.cc
	$(GCC) -c -o $@ $<

clean:
	- rm obj/*.o
	- rm $(EXECBIN)
	- rm -r *~
	- rm log.*
