CPP = g++
CPPFLAGS = -lncurses

build:
	$(CPP) main.cpp snake.cpp $(CPPFLAGS) -o nsnake

clean:
	rm -f ./nsnake
