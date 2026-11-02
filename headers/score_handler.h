#include "structs.h"
#include "input.h"
#include <stdio.h>

int handle_score(maze m);
int get_worst_score(FILE *file);
void create_score_file(char *path, char *player_name, int score);
void insert_score_in_file(FILE *file, char *player_name, int score);