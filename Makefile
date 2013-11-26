CC=gcc
HARDORE=-pedantic -ansi -Wall -Werror -pthread
CFILES=./src/main.c


all:
		$(CC) -pthread $(CFILES)  -o order

HARDCORE:
		$(CC) $(CFILES) $(HARDCORE) -o order

debug:
		$(CC) -g -pthread $(CFILES)  $(CFLAGS) -o order

clean:
		rm order
