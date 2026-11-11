#pragma once

#include "maze.h"
#include "structs.h"

void show_menu();
void display_maze(maze m);
void print_monster(monster);
void print_character(maze m, position pos);
void print_wall_character(maze m, position pos);
int show_file_list(char *list_name);
