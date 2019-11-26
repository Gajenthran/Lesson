CC=gcc
CFLAGS = -c -Wall
SRC=ann.c ll.c som.c parser.c
OBJ=$(SRC:.c=.o)

%.o:	%.c
	$(CC) $(CFLAGS) $<

ann: 	$(OBJ)
	$(CC) $(LINK) $(OBJ) -o $@

clean:
	rm *.o ann