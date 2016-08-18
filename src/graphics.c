#include "graphics.h"

static void logWindowDetails(const char * detail, SDL_DisplayMode *mode){
	printf("%s:\n  %ix%i @ %i\n  %i monitors\n",
		detail,
		mode->w, mode->h, mode->refresh_rate,
		SDL_GetNumVideoDisplays()
	);
}

static textureID areaTexture;
struct font *globalFont, *globalMonoFont;
static GLint LODlevel = 0;

void initiateGraphics(struct graphics *g, const char* name){
	if(SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO) < 0){
        printf("SDL failed to start: %s", SDL_GetError());
        return;
    }
	g->window = SDL_CreateWindow(
		name,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, g->width, g->height,
		SDL_WINDOW_OPENGL
	);
    setCameraOffset(g->width/2, g->height/2);

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
	g->windowHeight = mode.h;
	g->windowWidth = mode.w;
	glClearColor(0, 0, 0, 1);
	setVSync(0);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    globalFont = loadFont(assetFolderPath "font", 8, 16);
    globalMonoFont = loadFont(assetFolderPath "fontMono", 8, 16);
    areaTexture = loadTexture(assetFolderPath "areaHuge.png");
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

    SDL_SetSurfaceBlendMode(fullFont, SDL_BLENDMODE_NONE | SDL_BLENDMODE_BLEND);
    SDL_SetColorKey(fullFont, SDL_TRUE, SDL_MapRGB(fullFont->format, 0, 0, 0));

    for(int i = 0, ty = 0; ; ty++){
        for(int tx = 0; tx < width; tx++){
            SDL_Rect srcrect = {.x = tx * bits, .y = ty * bits,
                                .w = newFont->kerning[i], .h = bits};

            SDL_Surface *fontLetter = SDL_CreateRGBSurface(0, srcrect.w, srcrect.h, 32, 0xff000000, 0xff0000, 0xff00, 0xff);
            //TODO better way to copy textures
            SDL_FillRect(fontLetter, NULL, SDL_MapRGB(fontLetter->format, 0, 0, 0));
            SDL_BlitSurface(fullFont, &srcrect, fontLetter, NULL);

            newFont->chars[i] = loadTextureFromSurface(fontLetter);
            if(++i >= 128) goto FINISH;
        }
    }

FINISH:
    SDL_FreeSurface(fullFont);
    return newFont;
}

textureID loadTexture(const char *name){
    SDL_Surface *texture = IMG_Load(name);
    if(texture == NULL){
        printf("Failed to load image: %s\n", IMG_GetError());
        return -1;
    }
    return loadTextureFromSurface(texture);
}

textureID loadTextureFromSurface(SDL_Surface *texture){
    GLuint id = 0;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    int mode = texture->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, LODlevel, mode, texture->w, texture->h, 0, mode, GL_UNSIGNED_BYTE, texture->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);*/
    /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);*/

    SDL_FreeSurface(texture);
    return (textureID) id; //Cast for warning protection
}

void destroyGraphics(struct graphics *g){
    IMG_Quit();

	SDL_GL_DeleteContext(g->glcontext);
	SDL_DestroyWindow(g->window);
	SDL_Quit();
}

void renderSquareTextureRot(textureID textureid, int x, int y, int w, int h, angle ang){
    glBindTexture(GL_TEXTURE_2D, textureid);
    ang = ang + PI/2;

#define xm (x + w/2)
#define ym (y + h/2)

#define r(x, y) glVertex3i( \
    ((x - xm) * cos(ang) - (y - ym) * sin(ang)) + xm, \
    ((x - xm) * sin(ang) + (y - ym) * cos(ang)) + ym, \
    0)

    glBegin(GL_QUADS);
        glTexCoord2i(0, 0); r(x    , y    );
        glTexCoord2i(1, 0); r(x + w, y    );
        glTexCoord2i(1, 1); r(x + w, y + h);
        glTexCoord2i(0, 1); r(x    , y + h);
    glEnd();
}

void renderSquareTexture(textureID textureid, int x, int y, int w, int h){
    glBindTexture(GL_TEXTURE_2D, textureid);
    glBegin(GL_QUADS);
        glTexCoord2i(0, 0); glVertex3i(x    , y    , 0);
        glTexCoord2i(1, 0); glVertex3i(x + w, y    , 0);
        glTexCoord2i(1, 1); glVertex3i(x + w, y + h, 0);
        glTexCoord2i(0, 1); glVertex3i(x    , y + h, 0);
    glEnd();
}

static int renderedEnts = 0, renderedBullets = 0;
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

#define inRender(x, y, w, h) \
        x > -w && x < g->width  + h && \
        y > -w && y < g->height + h \

    renderedBullets = 0;
    for(pbull b = worldBullets; b != NULL; b = b->next){
        int x = (int) b->x - cameraX, y = (int) b->y - cameraY;
        struct bulletData par = bulletDatas[b->dataid];
        if(inRender(x, y, par.w, par.h)){
            renderSquareTextureRot(par.texture, x - par.w/2, y - par.h/2, par.w, par.h, b->ang);
            renderedBullets++;
        }
    }

    renderedEnts = 0;
    for(pent c = worldEntities.first; c != NULL; c = c->next){
        int x = (int) c->x - cameraX, y = (int) c->y - cameraY;
        if(inRender(x, y, c->w, c->h)){
            renderSquareTexture(c->textureID, x - c->w/2, y - c->h/2, c->w, c->h);
            renderedEnts++;
        }
    }
}

static void renderStatusBar(struct graphics *g, framerate frameTime, framerate appTime){
    glColor3f(1.0, 1.0, 1.0);

    char buf[128];
    int y = 0;
    framerate fps = MIN(1/frameTime, 1000);
    sprintf(buf, "%.2fs %.4f spf %.0f FPS", appTime, frameTime, fps);
    renderTextJust(globalMonoFont, buf, g->width, 0, 4, JUSTIFY_RIGHT);
    if(cameraFollowing){
        sprintf(buf, "%.1f, %.1f pxy", cameraFollowing->x, cameraFollowing->y);
        renderTextJust(globalMonoFont, buf, g->width, y+=32, 4, JUSTIFY_RIGHT);
    }
    int rmx, rmy;
    worldMousePosition(&rmx, &rmy);
    sprintf(buf, "%i %i, %i mxy %i, %i rmxy", mouseState, mouseX, mouseY, rmx, rmy);
    renderTextJust(globalMonoFont, buf, g->width, y+=32, 4, JUSTIFY_RIGHT);

    sprintf(buf, "E: %i B: %i T: %i", renderedEnts, renderedBullets, renderedBullets + renderedEnts);
    renderTextJust(globalMonoFont, buf, g->width, y+=32, 4, JUSTIFY_RIGHT);

#ifdef DEBUGFONT
#define TEST(a) renderTextJust(globalFont, a, g->width, y+=32, 4, JUSTIFY_RIGHT)
    TEST("cwm fjordbank glyphs vext quiz");
    TEST("CWM FJORDBANK GLYPHS VEXT QUIZ");
    TEST("Lorem ipsum dolor sit amet, consectetur adipiscing elit.");
    TEST(" !\"#$%&'()*+,-./0123456789:;<=>?");
    TEST("@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_");
    TEST("`abcdefghijklmnopqrstuvwxyz{|}~.");
#undef TEST
#endif
}

#define glRectiWH(x, y, w, h) renderSquareTexture(areaTexture, x, y, w, h)
/*#define glRectiWH(x, y, w, h) glRecti(x, y, (x) + (w), (y) + (h))*/

#define xl (infoBoxX + bu)
#define xr (infoBoxX + infoBoxWidth - bu)

#define PANELCOLOR glColor3f(.1, .1, .1)
#define WHITECOLOR glColor3f(1., 1., 1.)
#define BASICPANEL(height) \
    PANELCOLOR; \
    glRectiWH(xl, y, infoBoxWidth - dbu, height); \
    nexty = height;
#define ENDPANEL() y += nexty + bu;
#define ENDPANELW(a) y += a;

#define BUFFERI(var) sprintf(tbuf, "%i", var);

static void renderInterface(struct graphics *g){
    (void) g;
    pent lp;
    if((lp = cameraFollowing)){

        char tbuf[128];
        const int bu = 10;
        const int dbu = bu * 2;
        const int tbu = 4;
        const int dtbu = tbu * 2;
        const int infoBoxWidth = 200;
        const int infoBoxHeight = 400;
        const int infoBoxX = 0;
        const int infoBoxY = 20;

        int y = bu + infoBoxY, nexty;

        glColor3f(.2, .2, .2);
        glRectiWH(infoBoxX, infoBoxY, infoBoxWidth, infoBoxHeight);


#define DISBAR(amt, max, r, g, b) {\
        BASICPANEL(16 + dtbu); \
        float pct = (float) (amt) / (float) (max); \
        glColor3f(r, g, b); \
        glRectiWH(infoBoxX + bu, y, (int)((infoBoxWidth - dbu) * pct), 16 + dtbu); \
        glColor3f(1., 1., 1.); \
        BUFFERI(amt); \
        renderTextJust(globalFont, tbuf, xl + tbu, y + tbu, 2, JUSTIFY_LEFT); \
        sprintf(tbuf, "%.0f%%", pct * 100); \
        renderTextJust(globalFont, tbuf, xr - tbu, y + tbu, 2, JUSTIFY_RIGHT); \
        ENDPANEL(); \
        }


        DISBAR(lp->hp, getEntityMaxHealth(lp), .8, .0, .0);
        int amt;
        if((amt = getEntityMaxAbility(lp))){
            DISBAR(lp->abi, amt, .6, .0, 1.);
        }

        //Start stats panel
        int statsText = (16 + tbu);
        BASICPANEL(statsText * 4 + tbu);

        int xoffset = 0;

#define STDISPL(name, variable, r, g, b) \
        BUFFERI(variable); \
        WHITECOLOR; \
        xoffset = renderTextJust(globalFont, name " ", xl + tbu, y + tbu, 2, JUSTIFY_LEFT); \
        glColor3f(r, g, b); \
        renderTextJust(globalFont, tbuf, xl + tbu + xoffset, y + tbu, 2, JUSTIFY_LEFT);

#define STDISPR(name, variable, r, g, b) \
        BUFFERI(variable); \
        WHITECOLOR; \
        xoffset = renderTextJust(globalFont, " " name, xr - tbu, y + tbu, 2, JUSTIFY_RIGHT); \
        glColor3f(r, g, b); \
        renderTextJust(globalFont, tbuf, xr - tbu - xoffset, y + tbu, 2, JUSTIFY_RIGHT);

        STDISPL("AGI" , lp->stats.agi          , .0, 1., .3);
        STDISPR("MS"  , getEntityMovespeed(lp) , .5, .5, .5);
        y += statsText;

        STDISPL("VIT" , lp->stats.vit          , .8, .0, .0);
        STDISPR("HP"  , getEntityMaxHealth(lp) , .8, .0, .0);
        y += statsText;

        STDISPL("ABI" , lp->stats.abi          , .6, .0, 1.);
        STDISPR("POOL", getEntityMaxAbility(lp), .6, .0, 1.);
        y += statsText;

        STDISPL("DEF" , lp->stats.def          , .7, .7, .7);

        ENDPANELW(tbu + bu + statsText); //END stats panel

    }
}

void renderGraphics(struct graphics *g, framerate frameTime, framerate appTime){
#ifdef DEBUG
    glClear(GL_COLOR_BUFFER_BIT);
#endif

	glPushMatrix();
		glOrtho(0., g->width, g->height, 0., 0., 1.);
		glColor3f(1.0f, 1.0f, 1.0f);
        renderWorld2D(g);
		renderInterface(g);
        renderStatusBar(g, frameTime, appTime);
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
