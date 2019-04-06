CC=g++
CFLAGS=-std=c++17 -std=c++17 -pthread -lbenchmark # -DW=16 -DH=16
LINK=-O3
SRC= td2-gsimage.cpp td2-filtrage-thread.cpp td2-filtrage.cpp # player.cpp # sdl_driver.c
OBJ=$(SRC:.c=.o)

%.o:	%.c
	$(CC) -c $(CFLAGS) $< 

td2-img: 	$(OBJ)
	$(CC) $(LINK) $(OBJ) -o $@

clean:
	rm *.o game
