#include "structs.h"
#include "input.h"
#include <stdio.h>

int handle_score(maze m);
void create_score_file(char *path, char *player_name, int score);
void merge_score(int *score, char names[11][PLAYER_NAME_MAX_LENGTH], int left, int mid, int right);
void merge_sort_score(int *score, char names[11][PLAYER_NAME_MAX_LENGTH], int left, int right);