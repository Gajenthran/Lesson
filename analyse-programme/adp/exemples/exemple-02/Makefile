CC=gcc
CFLAGS=-Wall
SRC=../../adp.c ../../mco.c ../../match.c ../../list.c
OBJ=$(SRC:.c=.o)

%.o:	%.c
	$(CC) -c $(CFLAGS) $< 

adp: 	$(OBJ)
	$(CC) $(LINK) $(OBJ) -o $@

clean:
	rm *.o adp