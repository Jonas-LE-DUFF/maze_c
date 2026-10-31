#include "../headers/score_handler.h"

int handle_score(maze m) {
    char path[MAX_FILE_PATH_SIZE];
    int result = snprintf(path, sizeof(path), "score/%s.score", m.maze_name);

    if (result < 0 || (size_t)result >= sizeof(path)) {
        fprintf(stderr, "Erreur : le chemin du fichier est trop long\n");
        return 1;
    }
    printf("path : %s\n", path);

    char player_name[PLAYER_NAME_MAX_LENGTH];
    ask_for_string(PLAYER_NAME_MAX_LENGTH, "Enter your pseudo : ", player_name);

    FILE *f = fopen(path, "r+");
    if (f == NULL) {
        printf("No score found for this maze creating score file...\n");
        create_score_file(path, player_name, m.score);
        return 0;
    } else {
        int score[11];
        int score_i;
        char name_i[PLAYER_NAME_MAX_LENGTH];
        char names[11][PLAYER_NAME_MAX_LENGTH];
        int i = 0;
        fscanf(f, "### SCORE ###\n");
        while (i < 10 &&
               fscanf(f, "#%*d : %d : %9s\n", &score_i, name_i) == 2) {
            score[i] = score_i;
            memcpy(names[i], name_i, sizeof(char) * PLAYER_NAME_MAX_LENGTH);
            i++;
        }
        score[i] = m.score;
        memcpy(names[i], player_name, sizeof(char) * PLAYER_NAME_MAX_LENGTH);
        
        merge_sort_score(score, names, 0, i);

        fseek(f, sizeof("### SCORE ###"), SEEK_SET);
        for(int j = 0; j <= i && j < 10; j++){
            printf("row : %d\n", j);
            printf("score : %d\n", score[j]);
            printf("ligne : %s\n", names[j]);
            fprintf(f, "#%d : %3d : %s\n", j+1, score[j], names[j]);
        }
    }
    fclose(f);
    return 0;
}

void create_score_file(char *path, char *player_name, int score) {
    FILE *f = fopen(path, "a");
    if (f == NULL) {
        fprintf(stderr, "Erreur : échec de l'ouverture du fichier\n");
        return;
    }
    fprintf(f, "### SCORE ###\n");
    fprintf(f, "#1 : %3d : %s\n", score, player_name);
    fclose(f);
}

void merge_score(int *score, char names[11][PLAYER_NAME_MAX_LENGTH], int left, int mid, int right){
    int i, j, k;
    int left_length = mid - left + 1;
    int right_length = right - mid;

    int left_score[left_length];
    int right_score[right_length];
    char left_names[left_length][PLAYER_NAME_MAX_LENGTH];
    char right_names[right_length][PLAYER_NAME_MAX_LENGTH];

    for(i = 0; i < left_length; i++){
        left_score[i] = score[left + i]; 
        memcpy(left_names[i], names[left + i], sizeof(char) * PLAYER_NAME_MAX_LENGTH);
    }
    for(j = 0; j < right_length; j++){
        right_score[j] = score[mid + j + 1];
        memcpy(right_names[j], names[mid + j + 1], sizeof(char) * PLAYER_NAME_MAX_LENGTH);
    }

    i = 0;
    j = 0;
    k = left;
    while (i < left_length && j < right_length) {
        if (left_score[i] > right_score[j]) {
            score[k] = left_score[i];
            memcpy(names[k], left_names[i], sizeof(char) * PLAYER_NAME_MAX_LENGTH);
            i++;
        }
        else {
            score[k] = right_score[j];
            memcpy(names[k], right_names[j], sizeof(char) * PLAYER_NAME_MAX_LENGTH);
            j++;
        }
        k++;
    }

    while (i < left_length){
        score[k] = left_score[i];
        memcpy(names[k], left_names[i], sizeof(char) * PLAYER_NAME_MAX_LENGTH);
        i++;
        k++;
    }

    while (j < right_length){
        score[k] = right_score[j];
        memcpy(names[k], right_names[j], sizeof(char) * PLAYER_NAME_MAX_LENGTH);
        j++;
        k++;
    }
}

void merge_sort_score(int *score, char names[11][PLAYER_NAME_MAX_LENGTH], int left, int right){
    if(left < right){
        int mid = left + (right - left) / 2;

        merge_sort_score(score, names, left, mid);
        merge_sort_score(score, names, mid + 1, right);

        merge_score(score, names, left, mid, right);
    }
}