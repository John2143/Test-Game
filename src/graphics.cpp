#include "graphics.h"

static void logWindowDetails(const char *detail, SDL_DisplayMode *mode){
    printf("%s:\n  %ix%i @ %i\n  %i monitors\n",
        detail,
        mode->w, mode->h, mode->refresh_rate,
        SDL_GetNumVideoDisplays()
    );
}

static textureID areaTexture;
Font *globalFont, *globalMonoFont;

static GLint LODlevel = 0;
Graphics *currentGraphics;
Graphics::Graphics(const char *name, int width, int height): width(width), height(height){
    if(SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO) < 0){
        printf("SDL failed to start: %s", SDL_GetError());
        return;
    }
    this->window = SDL_CreateWindow(
        name,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
        SDL_WINDOW_OPENGL
    );
    setCameraOffset(width/2, height/2);

    if(!this->window){
        printf("Failed to create error: %s\n", SDL_GetError());
        return;
    }

    if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)){
        printf("Failed to load SDL_image: %s\n", IMG_GetError());
        return;
    }

    SDL_DisplayMode mode;
    SDL_GetWindowDisplayMode(this->window, &mode);
    logWindowDetails("Default", &mode);

    this->glcontext = SDL_GL_CreateContext(this->window);
    this->windowHeight = mode.h;
    this->windowWidth = mode.w;
    glClearColor(0, 0, 0, 1);

    SDL_GL_SetSwapInterval(1);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    globalFont =     new Font(assetFolderPath "font", 8, 16);
    globalMonoFont = new Font(assetFolderPath "fontMono", 8, 16);
    areaTexture = loadTexture(assetFolderPath "areaHuge.png");

    //TODO maybe return this to lua, or pass it to the function somehow?
    currentGraphics = this;
}

Graphics::~Graphics(){
    IMG_Quit();

    SDL_GL_DeleteContext(this->glcontext);
    SDL_DestroyWindow(this->window);
    SDL_Quit();
}

Font::Font(const char *name, int bits, int width){
    SDL_Surface *fullFont;
    this->bits = bits;

    {
        char buff[128];
        strcpy(buff, name);
        strcat(buff, ".png");
        fullFont = IMG_Load(buff);
        if(fullFont == NULL){
            printf("Failed to load image: %s\n", IMG_GetError());
            throw "Couldnt make font"_s;
        }
        strcpy(buff, name);
        strcat(buff, ".kerning");
        FILE *fp = fopen(buff, "rb");
        if(fp == NULL) throw "Couldnt open kerning"_s;
        size_t readBytes = fread(this->kerning, 1, KERNINGSIZE, fp);
        if(readBytes != KERNINGSIZE) throw "Couldnt read kerning properly"_s;
        fclose(fp);
    }

    SDL_SetSurfaceBlendMode(fullFont, (SDL_BlendMode) (SDL_BLENDMODE_NONE | SDL_BLENDMODE_BLEND));
    SDL_SetColorKey(fullFont, SDL_TRUE, SDL_MapRGB(fullFont->format, 0, 0, 0));

    for(int i = 0, ty = 0; ; ty++){
        for(int tx = 0; tx < width; tx++){
            SDL_Rect srcrect = {.x = tx * bits, .y = ty * bits,
                                .w = this->kerning[i], .h = bits};

            SDL_Surface *fontLetter = SDL_CreateRGBSurface(0, srcrect.w, srcrect.h, 32, 0xff000000, 0xff0000, 0xff00, 0xff);
            //TODO better way to copy textures
            SDL_FillRect(fontLetter, NULL, SDL_MapRGB(fontLetter->format, 0, 0, 0));
            SDL_BlitSurface(fullFont, &srcrect, fontLetter, NULL);

            this->chars[i] = Graphics::loadTextureFromSurface(fontLetter);
            if(++i >= 128) goto FINISH;
        }
    }

FINISH:
    SDL_FreeSurface(fullFont);
}

textureID loadTexture(const char *name){
    SDL_Surface *texture = IMG_Load(name);
    if(texture == NULL){
        printf("Failed to load image: %s\n", IMG_GetError());
        return -1;
    }
    return Graphics::loadTextureFromSurface(texture);
}

textureID Graphics::loadTextureFromSurface(SDL_Surface *texture){
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

void Graphics::renderSquareTextureRot(textureID textureid, int x, int y, int w, int h, angle ang){
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

void Graphics::renderSquareTexture(textureID textureid, int x, int y, int w, int h){
    glBindTexture(GL_TEXTURE_2D, textureid);
    glBegin(GL_QUADS);
        glTexCoord2i(0, 0); glVertex2i(x    , y    );
        glTexCoord2i(1, 0); glVertex2i(x + w, y    );
        glTexCoord2i(1, 1); glVertex2i(x + w, y + h);
        glTexCoord2i(0, 1); glVertex2i(x    , y + h);
    glEnd();
}

void Graphics::renderMap(){
    if(!World::currentWorld) return;
    World &world = *World::currentWorld;

    size_t startx, starty, endx, endy;
    if(cameraX < 0) startx = 0;
    else startx = MAX(cameraX / TILEPIXELS, 0);
    endx = MIN(((cameraX + this->width) / TILEPIXELS) + 1, world.size);

    if(cameraY < 0) starty = 0;
    else starty = MIN((cameraY / TILEPIXELS), 0);
    endy = MIN(((cameraY + this->height) / TILEPIXELS) + 1, world.size);

    for(size_t x = startx; x < endx; x++){
        for(size_t y = starty; y < endy; y++){
            this->renderSquareTexture(
                world.getd(x, y).texture,
                x * TILEPIXELS - cameraX,
                y * TILEPIXELS - cameraY,
                TILEPIXELS, TILEPIXELS
            );
        }
    }
}

static int renderedEnts = 0, renderedBullets = 0;
void Graphics::renderWorld2D(){
    this->renderMap();

#define inRender(x, y, w, h) \
        x > -w && x < this->width  + h && \
        y > -w && y < this->height + h \

    renderedBullets = 0;
    for(Bullet *b : worldBullets){
        int x = (int) b->x - cameraX, y = (int) b->y - cameraY;
        Bullet::bulletData par = b->getBaseData();
        if(inRender(x, y, par.w, par.h)){
            this->renderSquareTextureRot(par.texture, x - par.w/2, y - par.h/2, par.w, par.h, b->ang);
            renderedBullets++;
        }
    }

    renderedEnts = 0;
    for(Entity *c : worldEntities){
        int x = (int) c->x - cameraX, y = (int) c->y - cameraY;
        if(inRender(x, y, c->w, c->h)){
            this->renderSquareTexture(c->tid, x - c->w/2, y - c->h/2, c->w, c->h);
            renderedEnts++;
        }
    }
}

//This should be a power of two less than 256
#define NUMFRAMERATES 64
void Graphics::renderStatusBar(){
    static framerate frameRates[NUMFRAMERATES];
    static uint_least8_t curFramerate = 0;
    static bool hasInitializedFramerates = false;
    frameRates[curFramerate++ % NUMFRAMERATES] = frameTime;
    if(!hasInitializedFramerates && curFramerate > NUMFRAMERATES) hasInitializedFramerates = true;

    framerate normalizedFPS = 0, normalizedFrametime = 0;
    if(hasInitializedFramerates){
        for(int i = 0; i < NUMFRAMERATES; i++){
            normalizedFPS += MIN(1/frameRates[i], 1000);
            normalizedFrametime += frameRates[i];
        }
        normalizedFrametime /= NUMFRAMERATES;
        normalizedFPS /= NUMFRAMERATES;
    }

    glColor3f(1.0, 1.0, 1.0);

    char buf[128];
    int y = 0;
    framerate fps = MIN(1/frameTime, 1000);
    sprintf(buf, "%.2fs %.4f nspf %.0f NFPS", appTime, normalizedFrametime, normalizedFPS);
    this->renderTextJust(globalMonoFont, buf, this->width, 0, 4, JUSTIFY_RIGHT);
    sprintf(buf, "%.4f spf %.0f FPS", frameTime, fps);
    this->renderTextJust(globalMonoFont, buf, this->width, y+=32, 4, JUSTIFY_RIGHT);
    if(cameraFollowing){
        sprintf(buf, "%.1f, %.1f pxy", cameraFollowing->x, cameraFollowing->y);
        this->renderTextJust(globalMonoFont, buf, this->width, y+=32, 4, JUSTIFY_RIGHT);
    }
    int rmx, rmy;
    worldMousePosition(rmx, rmy);
    sprintf(buf, "%i %i, %i mxy %i, %i rmxy", mouseState, mouseX, mouseY, rmx, rmy);
    this->renderTextJust(globalMonoFont, buf, this->width, y+=32, 4, JUSTIFY_RIGHT);

    sprintf(buf, "E: %i B: %i T: %i", renderedEnts, renderedBullets, renderedBullets + renderedEnts);
    this->renderTextJust(globalMonoFont, buf, this->width, y+=32, 4, JUSTIFY_RIGHT);

#ifdef DEBUGFONT
#define TEST(a) this->renderTextJust(globalFont, a, g->width, y+=32, 4, JUSTIFY_RIGHT)
    TEST("cwm fjordbank glyphs vext quiz");
    TEST("CWM FJORDBANK GLYPHS VEXT QUIZ");
    TEST("Lorem ipsum dolor sit amet, consectetur adipiscing elit.");
    TEST(" !\"#$%&'()*+,-./0123456789:;<=>?");
    TEST("@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_");
    TEST("`abcdefghijklmnopqrstuvwxyz{|}~.");
#undef TEST
#endif
}

//Draws a square box with some texture
#define glRectiWH(x, y, w, h) this->renderSquareTexture(areaTexture, x, y, w, h)
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

#define BUFFERI(var) sprintf(tbuf, "%i", (int) (var));

void Graphics::renderInterface(){
    Entity *lp;
    if((lp = cameraFollowing)){

        char tbuf[128];
        //Buffer amount in pixels
        const int bu = 10;
        const int dbu = bu * 2;
        //Text buffer amount in pixels
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
        float pct = (amt) / (max); \
        glColor3f(r, g, b); \
        glRectiWH(infoBoxX + bu, y, (int)((infoBoxWidth - dbu) * pct), 16 + dtbu); \
        glColor3f(1., 1., 1.); \
        BUFFERI(amt); \
        this->renderTextJust(globalFont, tbuf, xl + tbu, y + tbu, 2, JUSTIFY_LEFT); \
        sprintf(tbuf, "%.0f%%", pct * 100); \
        this->renderTextJust(globalFont, tbuf, xr - tbu, y + tbu, 2, JUSTIFY_RIGHT); \
        ENDPANEL(); \
        }


        DISBAR(lp->hp, lp->getMaxHealth(), .8, .0, .0);
        int amt;
        if((amt = lp->getMaxAbility())){
            DISBAR(lp->abi, amt, .6, .0, 1.);
        }

        //Start stats panel

#define STDISPL(name, variable, r, g, b) \
        BUFFERI(variable); \
        WHITECOLOR; \
        xoffset = this->renderTextJust(globalFont, name " ", xl + tbu, y + tbu, 2, JUSTIFY_LEFT); \
        glColor3f(r, g, b); \
        this->renderTextJust(globalFont, tbuf, xl + tbu + xoffset, y + tbu, 2, JUSTIFY_LEFT);

#define STDISPR(name, variable, r, g, b) \
        BUFFERI(variable); \
        WHITECOLOR; \
        xoffset = this->renderTextJust(globalFont, " " name, xr - tbu, y + tbu, 2, JUSTIFY_RIGHT); \
        glColor3f(r, g, b); \
        this->renderTextJust(globalFont, tbuf, xr - tbu - xoffset, y + tbu, 2, JUSTIFY_RIGHT);

        int statsText = (16 + tbu);
        int xoffset = 0;
        BASICPANEL(statsText * 4 + tbu);

            STDISPL("AGI" , lp->stats.agi                , .0, 1., .3);
            STDISPR("MS"  , lp->getMovespeed()           , .5, .5, .5);
            y += statsText;

            STDISPL("VIT" , lp->stats.vit                , .8, .0, .0);
            STDISPR("HP"  , (int) lp->getMaxHealth()     , .8, .0, .0);
            y += statsText;

            STDISPL("ABI" , (int) lp->stats.abi          , .6, .0, 1.);
            STDISPR("POOL", (int) lp->getMaxAbility()    , .6, .0, 1.);
            y += statsText;

            STDISPL("DEF" , lp->stats.def          , .7, .7, .7);

        ENDPANELW(tbu + bu + statsText); //END stats panel

        if(lp->inv){
            const int sidebuff = 4;
            const int dsidebuff = sidebuff * 2;
            const int itemsize = 32;
            const int itembuffer = 6;

            Inventory *inv = lp->inv;
            xoffset = bu;
            bool incrementY = false;
            for(unsigned int i = 0; i < inv->size; i++){
                if(incrementY){
                    y += 32 + itembuffer + dsidebuff;
                    incrementY = false;
                }
                Item *it = inv->items[i];
                PANELCOLOR;
                glRectiWH(infoBoxX + xoffset, y, dsidebuff + itemsize, dsidebuff + itemsize);
                WHITECOLOR;
                if(it){
                    Item::itemData itd = it->getBaseData();
                    this->renderSquareTexture(itd.texture, infoBoxX + xoffset + sidebuff, y + sidebuff, itemsize, itemsize);
                }else{
                    const char *const STEXT[] = {
                        "M1", "M2", "SP",
                        "1", "2", "3", "4", "5",
                        "S1", "S2", "S3", "S4", "S5",
                    };
                    if(i < STRUCTARRAYLEN(STEXT)){
                        this->renderTextJust(globalFont, STEXT[i],
                            infoBoxX + xoffset + sidebuff + itemsize/2,
                            y + sidebuff + 4, 3, JUSTIFY_CENTER);
                    }
                }
                xoffset += itembuffer + itemsize + dsidebuff;
                if(xoffset + bu + itemsize + dsidebuff > infoBoxWidth){
                    xoffset = bu;
                    incrementY = true;
                }
            }
        }
    }
}

void Graphics::renderStart(){
    glClear(GL_COLOR_BUFFER_BIT);

    glPushMatrix();

    glOrtho(0., this->width, this->height, 0., 0., 1.);
    glColor3f(1.0f, 1.0f, 1.0f);
    this->renderWorld2D();

}
void Graphics::renderEnd(){
    glPopMatrix();

    SDL_GL_SwapWindow(this->window);
}

int Graphics::renderChar(Font *f, const unsigned char c, int x, int y, int scale){
    int ret = scale * f->kerning[c];
    this->renderSquareTexture(f->chars[c], x, y, ret, scale * f->bits);
    return ret;
}

int Graphics::renderText(Font *f, const char *text, int x, int y, int scale){
    const char *c = text;
    int len = 0;
    while(*c) len += this->renderChar(f, *c++, x + len, y, scale) + scale;
    return len - scale;
}

int Graphics::renderTextJust(const char *text, int x, int y, int scale, justification just){
    return this->renderTextJust(globalFont, text, x, y, scale, just);
}
int Graphics::renderTextJust(Font *f, const char *text, int x, int y, int scale, justification just){
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

int Graphics::textLength(Font *f, const char *text, int scale){
    const char *c = text;
    int len = 0;
    while(*c) len += scale * (f->kerning[(unsigned char) *c++] + 1);
    return len - scale;
}
