#pragma once

#include <stdlib.h>

#define MAX_MAZE_WIDTH 200
#define MAX_MAZE_HEIGHT 200
#define MAX_MAZE_NAME_SIZE 50
#define MAX_FILE_PATH_SIZE 74

#define PLAYER_NAME_MAX_LENGTH 30

#define TREASURE_BONUS 50
#define TRAP_MALUS 25

#define TREASURE_FREQUENCY 22
#define TRAP_FREQUENCY 12

#define SCORE_LOSS 1 // score lost with every move
#define SCORE_START 100

#define MONSTER_FREQUENCY 75

#define GHOST_PENALITY_POINTS 70
#define ORC_PENALITY_POINTS   100

#define GHOST_RADIUS 5
#define ORC_RADIUS   10
// new stats after the orc found a treasure
#define ORC_DETECTION_RADIUS 5
#define ORC_PATROL_RADIUS 2

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
    ERROR = -1, // used when players enter wrong input
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

typedef enum {
    GHOST = 0,
    ORC,
} monster_type;

struct maze;
struct monster;

typedef struct monster {
    monster_type monster_type;
    position monster_pos;
    position center;
    int radius;
    int penality_points;
    void (*movement)(struct maze *m, struct monster *monster);
} monster;

typedef struct maze {
    tile **map;
    int height;
    int width;
    int score;
    char maze_name[MAX_MAZE_NAME_SIZE];
    monster **monsters;
    int monster_amount;
} maze;