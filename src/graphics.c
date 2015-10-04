#include "graphics.h"

static void logWindowDetails(const char * detail, SDL_DisplayMode *mode){
	printf("%s:\n  %ix%i @ %i\n  %i monitors\n",
		detail,
		mode->w, mode->h, mode->refresh_rate,
		SDL_GetNumVideoDisplays()
	);
}

void initiateGraphics(struct graphics *g, const char* name){
	SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO);
	g->window = SDL_CreateWindow(
		name,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 480,
		SDL_WINDOW_OPENGL
	);

	SDL_DisplayMode mode;
	SDL_GetWindowDisplayMode(g->window, &mode);
	logWindowDetails("Default", &mode);

	g->glcontext = SDL_GL_CreateContext(g->window);
	glClearColor(0, 0, 0, 1);
}

void destroyGraphics(struct graphics *g){
	SDL_GL_DeleteContext(g->glcontext);
	SDL_DestroyWindow(g->window);
	SDL_Quit();
}

void renderGraphics(struct graphics *g){
	(void) g;
	glBegin(GL_QUADS);
		glVertex2f(-0.5f, -0.5f);
		glVertex2f(0.5f, -0.5f);
		glVertex2f(0.5f, 0.5f);
		glVertex2f(-0.5f, 0.5f);
	glEnd();
	SDL_GL_SwapWindow(g->window);
}
