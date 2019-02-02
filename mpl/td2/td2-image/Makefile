CC=g++
CFLAGS=-Wall -std=c++14 -O3 -lbenchmark -pthread `sdl2-config --cflags`
LINK=-Wall -std=c++14 -O3 -lbenchmark -pthread `sdl2-config --libs` -lSDL2_image -lSDL2_ttf
SRC=td2-gsimage.cpp td2-filtrage.cpp
OBJ=$(SRC:.c=.o)

%.o:	%.c
	$(CC) -c $(CFLAGS) $< 

td2-img: 	$(OBJ)
	$(CC) $(LINK) $(OBJ) -o $@

clean:
	rm *.o td2-img
