CC=gcc
HARDORE=-pedantic -ansi -Wall -Werror
CFILES=./src/main.c


all:
		$(CC) $(CFILES)  -o order

HARDCORE:
		$(CC) $(CFILES) $(HARDCORE) -o order

debug:
		$(CC) -g $(CFILES)  $(CFLAGS) -o order

clean:
		rm order
