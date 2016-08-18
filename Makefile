CC=gcc -std=c11
CFLAGS=-Wall -Wextra -pedantic -Iinclude
D=
FILES=game.c graphics.c input.c logic.c entity.c lua.c camera.c tile.c map.c bullet.c
SOURCES=$(addprefix src/, $(FILES))
EXECUTABLE=game
ifeq ($(shell uname),Linux)
LIBRARIES=-L. -lSDL2main -lSDL2 -lSDL2_image -lm -L./luasrc/src -llua -ldl -lGL
else
LIBRARIES=-L. -lSDL2main -lSDL2 -lOpengl32 -lglu32 -lSDL2_image -llua53
endif

OUTPUT=$(D) $(CFLAGS) $(SOURCES) $(LIBRARIES) -o $(EXECUTABLE)

all:
	$(CC) -DDEBUG -g $(OUTPUT)
prod:
	$(CC) -O3 $(OUTPUT)

clean:
	rm $(EXECUTABLE).exe
