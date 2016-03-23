#ifndef input_H
#define input_H

#include <stdint.h>
#include <SDL2/SDL.h>
#ifdef DEBUG
#	include <stdio.h>
#endif

#define MAXKEYSDOWN 16

void keyEvent(SDL_KeyboardEvent key);
int isKeyPressed(uint32_t code);

#endif
