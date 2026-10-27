#include "score_handler.h"

int handle_score(maze m) {
    char path[MAX_FILE_PATH_SIZE];
    snprintf(path, MAX_FILE_PATH_SIZE, "score/%s.score", m.maze_name);
    printf("path : %s\n", path);

    char player_name[10];
    ask_for_string(10, "Enter your pseudo : ", player_name);

    FILE *f = fopen(path, "r+");
    if (f == NULL) {
        printf("No score found for this maze creating score file...\n");
        create_score_file(path, player_name, m.score);
    } else {
        char line[1000];
        while (fread(line, 1, 1000, f)) {
            printf("ligne : %s\n", line);
        }
    }
    fclose(f);
    return 0;
}

void create_score_file(char *path, char *player_name, int score) {
    FILE *f = fopen(path, "a");
    if (f == NULL) {
        fprintf(stderr, "Erreur : échec de l'ouverture du fichier\n");
        return -1;
    }
    fprintf(f, "### SCORE ###\n");
    fprintf(f, "#1 : %d : %s\n", score, player_name);
    fclose(f);
}