CC=cc
CFLAGS=-lraylib -Wall
FILES=main.c fifo.c
NAME=uetg

all: clean uetg uetg.exe

uetg:
	$(CC) $(CFLAGS) $(FILES) -o $(NAME)

uetg.exe:
	echo win

web:
	# This likely works but I don't have raylib built from source with the right flags so I can't actually test this lol. ymmv
	emcc $(CFLAGS) $(FILES) -o $(NAME)

clean:
	rm uetg uetg.exe || true
