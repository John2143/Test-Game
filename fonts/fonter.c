#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#define BYTESPERPIXEL 3
#define MAPH 16
#define MAPW 16
#define SIZE MAPH*MAPW*BYTESPERPIXEL

#define MAPOFFSET 0x36
#define MAP

unsigned char spriteW, spriteH;
unsigned char *bitmapData;
uint32_t getPixelOffset(int sx, int sy, int ix, int iy){
	return (
		((MAPH - 1) - sy) * spriteH * MAPW * spriteW +
		sx * spriteW +
		((spriteH - 1) - iy) * MAPW * spriteW+
		ix
	) * BYTESPERPIXEL;
}
uint32_t getPixelData(uint32_t pixel){//_RGB
	/*printf("%ix%i p%ix%i %x \n", sx, sy, ix, iy, pixel);*/
	return
		bitmapData[pixel] |
		bitmapData[pixel + 1] << 8 |
		bitmapData[pixel + 2] << 16;
}

int main(int argc, char **argv){
	if(argc < 3)
		return 0;

	spriteW = atoi(argv[2]);
	spriteH = atoi(argv[3]);
	const int spritePixels = spriteW * spriteH;
	const uint32_t readSize = SIZE * spritePixels;
	bitmapData = malloc(readSize);
	printf("Size is %X\n", readSize);
	{
		FILE *bmp = fopen(argv[1], "rb");
		fseek(bmp, MAPOFFSET, SEEK_SET);
		fread(bitmapData, 1, readSize, bmp); //BGR, all data reversed, (first row = last data)
		fclose(bmp);
	}
	uint8_t *realData[MAPW][MAPH];
	for(int i = 0; i < MAPH; i++){
		for(int k = 0; k < MAPW; k++){
			int curWidth = 0;
			int startOffset = 0;
			int j = spriteW;
			while(--j){
				uint32_t dat = getPixelData(k, i, j, 0);
				if(!(dat & 0xff0000) && (dat & 0x0000ff)){//no red, >1 blue
					curWidth++;
					startOffset = j;
				}
			}
			printf("%2.2i-%2.2i: %i  %i\n", k, i, curWidth, startOffset);
			if(curWidth){
				int width = (curWidth + 7) / 8;
				realData[k][i] = malloc(spriteH * width);
				int ii, ik;
				for(ii = 0; ii < spriteH; ii++){
					for(ik = 0; ik < spriteW; ik++){
						realData[k][i][ii * width + 
					}
				}
			}
		}
	}
	(void) realData;

	free(bitmapData);
	return 0;
}
