CC=gcc
CFLAGS = -c -Wall -O3
SRC=ann.c som.c parser.c ll.c 
OBJ=$(SRC:.c=.o)

%.o:	%.c
	$(CC) $(CFLAGS) $<

ann: 	$(OBJ)
	$(CC) $(LINK) $(OBJ) -o $@

clean:
	rm *.o ann