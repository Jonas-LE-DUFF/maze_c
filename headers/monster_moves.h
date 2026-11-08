#pragma once

#include "position.h"
#include "maze.h"
#include <stdio.h>

void move_ghost(maze *m, monster *monster);
void move_orc(maze *m, monster *monster);
void remove_monster(maze *m, monster monster);
void init_monsters(maze *m);