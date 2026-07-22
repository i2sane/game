CC=cc
CFLAGS=-lraylib
FILES=main.c fifo.c
NAME=uttg

all: clean uttg uttg.exe

uttg:
	$(CC) $(CFLAGS) $(FILES) -o $(NAME)

uttg.exe:
	echo win

web:
	# This likely works but I don't have raylib built from source with the right flags so I can't actually test this lol. ymmv
	emcc $(CFLAGS) $(FILES) -o $(NAME)

clean:
	rm uttg uttg.exe || true
