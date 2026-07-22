CC=cc
CFLAGS=-lraylib
FILES=main.c
NAME=uttg

uttg:
	$(CC) $(CFLAGS) $(FILES) -o $(NAME)

uttg.exe:
	echo win

all: uttg uttg.exe
