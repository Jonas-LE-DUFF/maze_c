#pragma once

#include "structs.h"

position get_position_after_move(position pos, direction dir);
int is_wall_at(maze m, position pos);
tile *get_tile(maze m, position pos);

direction get_direction(char input);
position get_player_position(maze m);
tile *get_door_tile(maze m);
tile *get_start_position(maze m);
