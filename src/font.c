#include "font.h"

int renderChar(struct font *font, const unsigned char c, int x, int y, int scale){
	glBindTexture(GL_TEXTURE_2D, font->textures[c]);
	int width = getWidth(font, c) * scale;
	int height = getHeight(font, c) * scale;
	glBegin(GL_QUADS);
		glTexCoord2d(0, 0);
		glVertex2d(x, y + height);
		glTexCoord2d(1, 0);
		glVertex2d(x + width, y + height);
		glTexCoord2d(1, 1);
		glVertex2d(x + width, y);
		glTexCoord2d(0, 1);
		glVertex2d(x, y);
	glEnd();
	printf("%i %i  %i   \r", font->textures[c], y, c);
	return width;
}

char getWidth(struct font *f, const unsigned char c){
	if(f->monospace)
		return f->monospace;
	else
		return f->width[c];
}
char getHeight(struct font *f, const unsigned char c){
	(void) f;
	(void) c;
	return 8;
}

typedef uint32_t fontDataType;

//This function is a bit silly
struct font *loadFont(const char* path){
	//Used to hold the whole font file
	char *string;
#if 1
	(void) path;
	string = malloc(ASCII_RANGE);
	string[8]  = 0b00000000;
	string[9]  = 0b01111110;
	string[10] = 0b01001010;
	string[11] = 0b01001010;
	string[12] = 0b00110110;
	string[13] = 0b00000000;
	string[14] = 0b00000000;
	string[15] = 0b00000000;
#else
	{
		FILE *f = fopen(path, "rb");
		fseek(f, 0, SEEK_END);
		long fsize = ftell(f);
		fseek(f, 0, SEEK_SET);

		string = malloc(fsize + 1);
		fread(string, fsize, 1, f);
		fclose(f);
	}
#endif
	struct font *n = malloc(sizeof(*n));
	//Used for texture ids
	//ascii char n = font->fOffset + n
	int i;
	if(1){//TODO
		n->monospace = 8;
	}else{
		n->monospace = 0;
		//Individual character width
		n->width = malloc(ASCII_RANGE);
		for(i = 0; i < ASCII_RANGE; i++){
			n->width[i] = 8; //TODO
		}
	}
	int stringpos = 0;
	glGenTextures(ASCII_RANGE, n->textures);
	for(i = 0; i < ASCII_RANGE; i++){
		glBindTexture(GL_TEXTURE_2D, n->textures[i]);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 2);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		char width = getWidth(n, i);

		//Matrix transform
		//  Each char in the string represents 8 vertical binary pixels.
		//  Iterate across each and set the actual pixel data in the texture
		//  and set each data pixel to an rgba representation in a unsigned short
		printf("Glyph %i(%i)\n", i, n->textures[i]);
		fontDataType data[8 * 2][width * 2];
		int j, k;
		int elemx, elemy;
		/*memset(data, 0, sizeof(data));*/
		for(j = 0; j < width; j++){
			elemy = j * 2;
			char shift = string[stringpos++];
			for(k = 0; k < 8; k++){
				fontDataType dat = shift & 1 << k ? 0x000000ff : 0xffffffff;
				elemx = k * 2;
				data[elemx    ][elemy    ] = dat;
				data[elemx + 1][elemy    ] = dat;
				data[elemx    ][elemy + 1] = dat;
				data[elemx + 1][elemy + 1] = dat;
			}
		}
		stringpos = 8; //------------------------------------------------------------

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width * 2, 8 * 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}

	free(string);

	return n;
}

void freeFont(struct font *f){
	if(!f->monospace)
		free(f->width);
	glDeleteTextures(ASCII_RANGE, f->textures);
	printf("Font freed\n");
	free(f);
}
