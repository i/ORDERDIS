CC=gcc
CFLAGS=-pedantic -ansi -Wall -Werror
CFILES=./src/main.c


all:
		$(CC) $(CFILES) $(CFLAGS) -o order

debug:
		$(CC) -g $(CFILES)  $(CFLAGS) -o order

clean:
		rm order
