CC=g++
CFLAGS=-std=c++17 -DW=2 -DH=2
LINK=-O3
SRC=grid.cpp connect4.cpp # grid.cpp player.cpp # sdl_driver.c
OBJ=$(SRC:.cpp=.o)

%.o:	%.cpp
	$(CC) -c $(CFLAGS) $< 

game: 	$(OBJ)
	$(CC) $(LINK) $(OBJ) -o $@

clean:
	rm *.o game