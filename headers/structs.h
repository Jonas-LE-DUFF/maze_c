#pragma once

#include <stdlib.h>

#define MAZE_WIDTH 25
#define MAZE_HEIGHT 11

#define MAX_MAZE_WIDTH 1000
#define MAX_MAZE_HEIGHT 1000
#define MAX_MAZE_NAME_SIZE 50
#define MAX_FILE_PATH_SIZE 74

#define TREASURE_BONUS 50
#define TRAP_MALUS 25

#define TREASURE_FREQUENCY 22
#define TRAP_FREQUENCY 12

#define SCORE_LOSS 1
#define SCORE_START 100

typedef enum {
    EMPTY = 0,
    WALL,
    PLAYER,
    EXIT,
    LOCKED_DOOR,
    KEY,
    TREASURE,
    TRAP,
    // TODO : ADD MONSTERS
} tile;

typedef struct {
    int row;
    int column;
} position;

typedef enum {
    ERROR = -1,
    UP = 0,
    RIGHT,
    DOWN,
    LEFT,
} direction;

typedef enum {
    QUIT = 0,
    CREATE_MAZE,
    LOAD_MAZE,
    PLAY,
    SCORE_BOARD,
} action;

typedef struct {
    tile **map;
    int height;
    int width;
    int score;
    char maze_name[MAX_MAZE_NAME_SIZE];
} maze;