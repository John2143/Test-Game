#ifndef input_H
#define input_H

#include "global.h"

#define MAXKEYSDOWN 16

extern int mouseX, mouseY;
extern Uint32 mouseState, oldMouseState;

void mouseEvent();
void keyEvent(SDL_KeyboardEvent key);
int isKeyPressed(uint32_t code);
extern void logicOnKeyPress(uint32_t code);

extern uint32_t keysdown[MAXKEYSDOWN];

#endif
