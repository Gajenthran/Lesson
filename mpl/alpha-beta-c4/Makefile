CC=g++
CFLAGS=-Wall -std=c++17 -DW=7 -DH=6
LINK=-O3
SRC=grid.cpp connect4.cpp solver.cpp
OBJ=$(SRC:.cpp=.o)

%.o:	%.cpp
	$(CC) -c $(CFLAGS) $< 

game: 	$(OBJ)
	$(CC) $(LINK) $(OBJ) -o $@

clean:
	rm *.o game