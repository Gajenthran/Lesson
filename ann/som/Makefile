CC=gcc
CFLAGS=-Wall
SRC=ann.c ll.c som.c parser.c
OBJ=$(SRC:.c=.o)

%.o:	%.c
	$(CC) -c $(CFLAGS) $< 

ann: 	$(OBJ)
	$(CC) $(LINK) $(OBJ) -o $@

clean:
	rm *.o ann