#include "graphics.h"

static void logWindowDetails(const char * detail, SDL_DisplayMode *mode){
	printf("%s:\n  %ix%i @ %i\n  %i monitors\n",
		detail,
		mode->w, mode->h, mode->refresh_rate,
		SDL_GetNumVideoDisplays()
	);
}

static textureID texture;
struct font *globalFont;
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

    texture = loadTexture(assetFolderPath "meme.png");
    globalFont = loadFont(assetFolderPath "font", 8, 16);
}

struct font *loadFont(const char *name, int bits, int width){
    SDL_Surface *fullFont;
    struct font *newFont = malloc(sizeof(struct font));
    newFont->bits = bits;

    {
        char buff[128];
        strcpy(buff, name);
        strcat(buff, ".png");
        fullFont = IMG_Load(buff);
        if(fullFont == NULL){
            printf("Failed to load image: %s\n", IMG_GetError());
            free(newFont);
            return NULL;
        }
        strcpy(buff, name);
        strcat(buff, ".kerning");
        FILE *fp = fopen(buff, "rb");
        if(fp == NULL){
            printf("Failed to load kerning\n");
            free(newFont);
            return NULL;
        }
        fread(newFont->kerning, 1, 128, fp);
        fclose(fp);
    }

    int dataMode = fullFont->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB;
    int pixelBits = fullFont->format->BitsPerPixel;

    for(int i = 0, ty = 0; ; ty++){
        for(int tx = 0; tx < width; tx++){
            SDL_Rect srcrect = {.x = tx * bits,
                                .y = ty * bits,
                                .w = newFont->kerning[i], .h = bits};

            SDL_Surface *fontLetter = SDL_CreateRGBSurface(0, srcrect.w, srcrect.h, pixelBits, 0, 0, 0, 0);
            SDL_BlitSurface(fullFont, &srcrect, fontLetter, NULL);

            glGenTextures(1, &newFont->chars[i]);
            glBindTexture(GL_TEXTURE_2D, newFont->chars[i]);

            glTexImage2D(GL_TEXTURE_2D, 0, dataMode, fontLetter->w, fontLetter->h, 0, dataMode, GL_UNSIGNED_BYTE, fontLetter->pixels);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            SDL_FreeSurface(fontLetter);

            if(++i >= 128) goto FINISH;
        }
    }

FINISH:
    SDL_FreeSurface(fullFont);
    return newFont;
}

textureID loadTexture(const char *name){
    SDL_Surface *texture = IMG_Load(name);

    GLuint id = 0;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    int mode = texture->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, LODlevel, mode, texture->w, texture->h, 0, mode, GL_UNSIGNED_BYTE, texture->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    SDL_FreeSurface(texture);
    return (textureID) id; //Cast for warning protection
}

void destroyGraphics(struct graphics *g){
    IMG_Quit();

	SDL_GL_DeleteContext(g->glcontext);
	SDL_DestroyWindow(g->window);
	SDL_Quit();
}

void renderSquareTexture(textureID textureid, int x, int y, int w, int h){
    glBindTexture(GL_TEXTURE_2D, textureid);
    glBegin(GL_QUADS);
        glTexCoord2i(0, 0); glVertex3i(x, y, 0);
        glTexCoord2i(1, 0); glVertex3i(x + w, y, 0);
        glTexCoord2i(1, 1); glVertex3i(x + w, y + h, 0);
        glTexCoord2i(0, 1); glVertex3i(x, y + h, 0);
    glEnd();
}

static void renderWorld2D(struct graphics *g){

    int startx, starty, endx, endy;
    startx = (cameraX / TILEPIXELS);
    endx   = ((cameraX + g->width) / TILEPIXELS) + 1;
    startx = MAX(startx, 0);
    endx   = MIN(endx, WORLDSIZE);

    starty = (cameraY / TILEPIXELS);
    endy   = ((cameraY + g->height) / TILEPIXELS) + 1;
    starty = MAX(starty, 0);
    endy   = MIN(endy, WORLDSIZE);
    for(int x = startx; x < endx; x++){
        for(int y = starty; y < endy; y++){
            renderSquareTexture(
                globalTileArray[gameworld[x][y]].texture,
                x * TILEPIXELS - cameraX,
                y * TILEPIXELS - cameraY,
                TILEPIXELS, TILEPIXELS
            );
        }
    }

    pent c = worldEntities.first;
    while(c != NULL){
        int x = (int) c->x - cameraX, y = (int) c->y - cameraY;
        renderSquareTexture(c->textureID, x, y, c->w, c->h);
        c = c->next;
    }
}

static void renderStatusBar(struct graphics *g, framerate frameTime){
    char buf[128];
    sprintf(buf, "%.0f FPS", 1 / frameTime);
    renderTextJust(globalFont, buf, g->width, 0, 4, JUSTIFY_RIGHT);
}

static void renderInterface(struct graphics *g){
    (void) g;
}

void renderGraphics(struct graphics *g, framerate frameTime){
	glClear(GL_COLOR_BUFFER_BIT);

	glPushMatrix();
		glOrtho(0., g->width, g->height, 0., 0., 1.);
		glColor3f(1.0f, 1.0f, 1.0f);
        renderWorld2D(g);
		renderInterface(g);
        renderStatusBar(g, frameTime);
	glPopMatrix();

	SDL_GL_SwapWindow(g->window);
}

int renderChar(const struct font *f, const unsigned char c, int x, int y, int scale){
    int ret = scale * f->kerning[c];
    renderSquareTexture(f->chars[c], x, y, ret, scale * f->bits);
    return ret;
}

int renderText(const struct font *f, const char *text, int x, int y, int scale){
    const char *c = text;
    int len = 0;
    while(*c) len += renderChar(f, *c++, x + len, y, scale) + scale;
	return len - scale;
}

int renderTextJust(const struct font *f, const char *text, int x, int y, int scale, enum justification just){
    int len = textLength(f, text, scale);
    switch(just){
        case JUSTIFY_RIGHT:
            return renderText(f, text, x - len, y, scale);
        case JUSTIFY_CENTER:
            return renderText(f, text, x - len/2, y, scale);
        case JUSTIFY_LEFT:
            return renderText(f, text, x, y, scale);
        default: return 0;
    }
}

int textLength(const struct font *f, const char *text, int scale){
    const char *c = text;
    int len = 0;
    while(*c) len += scale * (f->kerning[(unsigned char) *c++] + 1);
    return len - scale;
}

void setVSync(bool vsync){
	SDL_GL_SetSwapInterval(!!vsync);
}
