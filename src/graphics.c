#include "graphics.h"

static void logWindowDetails(const char * detail, SDL_DisplayMode *mode){
	printf("%s:\n  %ix%i @ %i\n  %i monitors\n",
		detail,
		mode->w, mode->h, mode->refresh_rate,
		SDL_GetNumVideoDisplays()
	);
}

static GLuint texture;
static GLint LODlevel = 0;

void initiateGraphics(struct graphics *g, const char* name){
	if(SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO) < 0){
        printf("SDL failed to start: %s", SDL_GetError());
        return;
    }
	g->window = SDL_CreateWindow(
		name,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1200, 900,
		SDL_WINDOW_OPENGL
	);
    setCameraOffset(1200/2, 900/2);

    if(!g->window){
        printf("Failed to create error: %s\n", SDL_GetError());
        return;
    }
    if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)){
        printf("Failed to load SDL_image: %s\n", IMG_GetError());
        return;
    }

	SDL_DisplayMode mode;
	SDL_GetWindowDisplayMode(g->window, &mode);
	logWindowDetails("Default", &mode);

	g->glcontext = SDL_GL_CreateContext(g->window);
	g->height = mode.h;
	g->width = mode.w;
	glClearColor(0, 0, 0, 1);
	setVSync(0);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    texture = loadTexture("assets/meme.png");
}

GLuint loadTexture(const char *name){
    SDL_Surface *texture = IMG_Load(name);

    GLuint id = 0;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    int mode = texture->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, LODlevel, mode, texture->w, texture->h, 0, mode, GL_UNSIGNED_BYTE, texture->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    /*printf("Loaded %s: %s\n", name, mode == GL_RGBA ? "RGBA" : "RGB");*/
    SDL_FreeSurface(texture);
    return id;
}

void destroyGraphics(struct graphics *g){
    IMG_Quit();

	SDL_GL_DeleteContext(g->glcontext);
	SDL_DestroyWindow(g->window);
	SDL_Quit();
}

static void renderWorld(){
}

static void renderWorld2D(){
    pent c = worldEntities.first;
    while(c != NULL){
        glBindTexture(GL_TEXTURE_2D, c->textureID);
        int x = (int) c->x - cameraX, y = (int) c->y - cameraY;
        glBegin(GL_QUADS);
            glTexCoord2i(0, 0); glVertex3i(x, y, 0);
            glTexCoord2i(1, 0); glVertex3i(x + c->w, y, 0);
            glTexCoord2i(1, 1); glVertex3i(x + c->w, y + c->h, 0);
            glTexCoord2i(0, 1); glVertex3i(x, y + c->h, 0);
        glEnd();
        c = c->next;
    }
}

static void renderInterface(){
}

void renderGraphics(struct graphics *g){
	glClear(GL_COLOR_BUFFER_BIT);

	renderWorld();

	glPushMatrix();
		glOrtho(0., g->width, g->height, 0., 0., 1.);
		glColor3f(1.0f, 1.0f, 1.0f);
        renderWorld2D();
		renderInterface();
	glPopMatrix();

	SDL_GL_SwapWindow(g->window);
}

int renderChar(char text, int x, int y, int scale){
    (void) text;
    (void) (x + y + scale);
    return 1;
}

int renderText(const char *text, int x, int y, int scale){
	for(int i = 0; text[i] != '\0'; i++){
		x += renderChar(text[i], x, y, scale);
	}
	return x; // x - xinitial?
}

void setVSync(bool vsync){
	SDL_GL_SetSwapInterval(!!vsync);
}
