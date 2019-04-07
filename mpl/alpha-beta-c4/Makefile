CC=g++
CFLAGS=-Wall -std=c++17 -DW=5 -DH=5
LINK=-O3 -pthread
SRC=grid.cpp connect4.cpp solver.cpp
OBJ=$(SRC:.cpp=.o)

%.o:	%.cpp
	$(CC) -c $(CFLAGS) $<

connect4: 	$(OBJ)
	$(CC) $(LINK) $(OBJ) -o $@

clean:
	rm -rf *.o connect4