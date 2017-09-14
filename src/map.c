#include "map.h"

tileid gameworld[WORLDSIZE][WORLDSIZE];

void brush(int x, int y, int size, tileid id){
    for(int mx = x - size; mx < x + size; mx++){
        for(int my = y - size; my < y + size; my++){
            gameworld[CLAMPWORLD(mx)][CLAMPWORLD(my)] = id;
        }
    }
}

static struct vector2I generateRandomRoomSize(int roomMult, double thetaBound){
    double theta = randomFloat(thetaBound, PI/2 - thetaBound);
    struct vector2I dimensions;
    dimensions.x = cos(theta) * roomMult;
    dimensions.y = sin(theta) * roomMult;
    return dimensions;
}

struct room{
    int x;
    int y;
    int w;
    int h;
};

void implantRoom(const struct room *room){
    for(int i = room->x; i < room->x + room->w; i++){
        gameworld[i][room->y] = 1;
        gameworld[i][room->y + room->h - 1] = 1;
    }
    for(int i = room->y; i < room->y + room->h; i++){
        gameworld[room->x][i] = 1;
        gameworld[room->x + room->w - 1][i] = 1;
    }

    for(int i = room->x + 1; i < room->x + room->w - 1; i++){
        for(int j = room->y + 1; j < room->y + room->h - 1; j++){
            gameworld[i][j] = 3;
        }
    }
}

void initializeWorld(){
    memset(&gameworld, 0, sizeof(tileid) * WORLDSIZE * WORLDSIZE);
    struct room room;
    int i = 3;
    while(i--){
        struct vector2I size = generateRandomRoomSize(30, PI/8);
        room.w = size.x;
        room.h = size.y;
        room.x = rand() % (WORLDSIZE - room.w - 2) + 1;
        room.y = rand() % (WORLDSIZE - room.h - 2) + 1;
        printf("%ix%i @ %i, %i", room.w, room.h, room.x, room.y);
        implantRoom(&room);
    }
}

void uninitializeWorld(){
}
