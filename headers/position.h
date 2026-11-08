#pragma once

#include "structs.h"

position get_position_after_move(position pos, direction dir);
int is_wall_at(maze m, position pos);
tile *get_tile(maze m, position pos);

direction get_direction(char input);
position get_player_position(maze m);
tile *get_door_tile(maze m);
tile *get_start_position(maze m);

int pos_equals(position pos, position other);
monster *get_monster_at_pos(maze *m, position pos);
int is_in_perimeter(position test, position center, int radius);
int get_distance(position pos, position other);
int is_border(maze m, position pos);
int is_occupied(maze m, position pos);
position get_closest_treasure(maze m, position pos);