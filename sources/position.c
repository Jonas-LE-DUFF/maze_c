#include "position.h"
#include <stddef.h>
#include <stdio.h>

position get_position_after_move(position pos, direction dir) {
    switch (dir) {
    case UP:
        return (position){pos.row - 1, pos.column};
    case RIGHT:
        return (position){pos.row, pos.column + 1};
    case DOWN:
        return (position){pos.row + 1, pos.column};
    case LEFT:
        return (position){pos.row, pos.column - 1};
    default:
        return pos;
        break;
    }
}

int is_wall_at(maze m, position pos) {
    tile *current = get_tile(m, pos);

    return current != NULL && (*current == WALL || *current == LOCKED_DOOR);
}

tile *get_tile(maze m, position pos) {
    if (pos.row < 0 || pos.column < 0)
        return NULL;
    if (pos.row >= (int)m.height || pos.column >= (int)m.width)
        return NULL;
    return &m.map[pos.row][pos.column];
}

direction get_direction(char input) {
    switch (input) {
    case 'Z':
        return UP;
    case 'Q':
        return LEFT;
    case 'S':
        return DOWN;
    case 'D':
        return RIGHT;
    default:
        fprintf(stderr, "Erreur : l'entrée ne fait pas partie de z,q,s,d : %c\n", input);
        return ERROR;
    }
}

position get_player_position(maze m) {
    for (int i = 0; i < m.height; i++) {
        for (int j = 0; j < m.width; j++) {
            if (m.map[i][j] == PLAYER) {
                return (position){i, j};
            }
        }
    }
    return (position){-1, -1};
}

tile *get_door_tile(maze m) {
    return get_tile(m, (position){m.height - 1, m.width - 2});
}

tile *get_start_position(maze m) {
    return get_tile(m, (position){0, 1});
}