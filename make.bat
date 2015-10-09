@echo off
gcc -Wall -Wextra ^
	-g -DDEBUG ^
	-L. -Iinclude ^
	src/game.c ^
	src/font.c ^
	src/graphics.c ^
	src/input.c ^
	src/logic.c ^
	-lSDL2main -lSDL2 ^
	-lOpengl32 -lglu32 ^
	-o game.exe
