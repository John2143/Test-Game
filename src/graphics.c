#include "graphics.h"

static void logWindowDetails(const char * detail, SDL_DisplayMode *mode){
	printf("%s:\n  %ix%i @ %i\n  %i monitors\n",
		detail,
		mode->w, mode->h, mode->refresh_rate,
		SDL_GetNumVideoDisplays()
	);
}

static struct font *debugFont;

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
	g->height = mode.h;
	g->width = mode.w;
	glClearColor(1, 0, 0, 1);
	setVSync(0);
	glEnable(GL_TEXTURE_2D);
	debugFont = loadFont(""); //TODO
}

void destroyGraphics(struct graphics *g){
	SDL_GL_DeleteContext(g->glcontext);
	SDL_DestroyWindow(g->window);
	SDL_Quit();
}

void renderWorld(){
	double spos, cpos;
	spos = sin(pos) * .5f;
	cpos = cos(pos) * .5f;

	glColor3f(0, spos + .5f, cpos + .5f);
	glBegin(GL_QUADS);
		glVertex2f(-spos, -cpos);
		glVertex2f(spos, -cpos);
		glVertex2f(spos, cpos);
		glVertex2f(-spos, cpos);
	glEnd();

	glColor3f(cpos + .5f, cpos + .5f, cpos + .5f);
	glBegin(GL_QUADS);
		glVertex2f(0.5f, 0.5f);
		glVertex2f(1.0f, 0.5f);
		glVertex2f(1.0f, 1.0f);
		glVertex2f(0.5f, 1.0f);
	glEnd();
}
void renderInterface(){
	renderText("Abcde", debugFont, 20, 20, 5);
	char fText[8];
	snprintf(fText, 7, "%.1f", fps);
	fText[8] = '\0';
	renderText(fText, debugFont, 0, 0, 2);
}

void renderGraphics(struct graphics *g){
	glClear(GL_COLOR_BUFFER_BIT);

	renderWorld();

	glPushMatrix();
		glOrtho(0., g->width, g->height, 0., 0., 1.);
		glColor3f(1.0f, 1.0f, 1.0f);
		renderInterface();
	glPopMatrix();

	SDL_GL_SwapWindow(g->window);
}

int renderText(const char *text, struct font *f, int x, int y, int scale){
	int i = 0;
	for(;text[i] != '\0'; i++){
		x += renderChar(f, text[i], x, y, scale);
	}
	return x; // x - xinitial?
}

void setVSync(char vsync){
	SDL_GL_SetSwapInterval(!!vsync);
}
