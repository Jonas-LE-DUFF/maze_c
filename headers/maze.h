#pragma once

#include "frontend.h"
#include "player_actions.h"
#include "position.h"
#include "structs.h"
#include "union_find.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void free_maze(maze *maze);
int generate_perfect_maze(maze *m, int height, int width);
int treat_wall(position wall_to_remove, maze *m, node **room_values, direction dir,
               direction opposed_dir);
int init_maze(maze *m, int height, int width);
void add_items(maze *m);

void shuffle(void *array, int n, int size);