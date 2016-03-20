CC=gcc -std=c11
CFLAGS=-Wall -Wextra -pedantic -Iinclude
FILES=game.c graphics.c input.c logic.c
SOURCES=$(addprefix src/, $(FILES))
EXECUTABLE=game
LIBRARIES=-L. -lSDL2main -lSDL2 -lOpengl32 -lglu32 -lSDL2_image

all:
	$(CC) -DDEBUG -g $(CFLAGS) $(SOURCES) $(LIBRARIES) -o $(EXECUTABLE)
prod:
	$(CC) -O3 $(CFLAGS) $(SOURCES) $(LIBRARIES) -o $(EXECUTABLE)

clean:
	rm *.o