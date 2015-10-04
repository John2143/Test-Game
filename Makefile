all:
	gcc -Wall -Wextra \
		-g -DDEBUG \
		-L. -Iinclude \
		src/game.c \
		src/graphics.c \
		src/input.c \
		src/logic.c \
		-lSDL2main -lSDL2 \
		-lOpengl32 -lglu32 \
		-o game.exe
clean:
	rm game.exe
	rm a.exe
