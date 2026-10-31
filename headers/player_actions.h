#pragma once

#include "input.h"
#include "maze.h"
#include "score_handler.h"
#include <dirent.h>

void show_menu();
int player_interact(maze *m);
void create_maze(maze *m);
int show_file_list(char *list_name);
void load_maze(maze *m);
void file_maze_load(maze *m);
int handle_move(maze *m, direction dir);
void save_maze(maze *m, char *path);
void play(maze *m);
void show_score_board();