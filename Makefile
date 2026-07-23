CC=cc
CFLAGS=-Wall
LDFLAGS=-L/usr/local/lib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
WEBLDFLAGS=-I/usr/local/include -Lweblib -lraylib 
FILES=main.c fifo.c
NAME=uetg

all: clean uetg uetg.exe web

uetg:
	$(CC) $(CFLAGS) $(FILES) -o $(NAME) $(LDFLAGS)

uetg.exe:
	echo win

web:
	# I love the web. This was definitely not something I struggled terribly with. I :heart: Brandon Eich
	mkdir webbuild || true
	emcc --preload-file assets -s USE_GLFW=3 -s ASYNCIFY $(CFLAGS) $(FILES) -o webbuild/$(NAME).html $(WEBLDFLAGS)  

clean:
	rm -rf webbuild || true
	rm uetg uetg.exe || true
