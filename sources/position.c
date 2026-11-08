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

int pos_equals(position pos, position other){
    return pos.column == other.column && pos.row == other.row;
}

monster *get_monster_at_pos(maze *m, position pos){
    for (int i = 0; i < m->monster_amount; i++)
    {
        monster *monster = m->monsters[i];
        if (pos_equals(monster->monster_pos, pos))
        {
            return monster;
        }
    }
    return NULL;
}

int get_distance(position pos, position other){
    return abs(pos.column - other.column)
        +  abs(pos.row    - other.row   );
}

int is_in_perimeter(position test, position center, int radius){
    return abs(test.column - center.column) <= radius
        && abs(test.row    - center.row   ) <= radius;
}

int is_border(maze m, position pos){
    return pos.column == 0 || pos.row == 0 || pos.column == m.width-1 || pos.row == m.height-1;
}

int is_occupied(maze m, position pos){
    for(int i = 0; i < m.monster_amount; i++){
        if(pos_equals(m.monsters[i]->monster_pos, pos)){
            return 1;
        }
    }
    return 0;
}

position get_closest_treasure(maze m, position pos){
    int min_distance = m.height * m.width;
    position closest = {-1, -1};
    for (int i = 0; i < m.height; i++)
    {
        for (int j = 0; j < m.width; j++)
        {
            position tested_pos = {i, j};
            if(*get_tile(m, tested_pos) == TREASURE
            && get_distance(tested_pos, pos) < min_distance){
                min_distance = get_distance(tested_pos, pos);
                closest = tested_pos;
            }
        }
    }
    return closest;
}