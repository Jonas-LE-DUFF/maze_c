#include "../headers/score_handler.h"

int handle_score(maze m) {
    char path[MAX_FILE_PATH_SIZE];
    int result = snprintf(path, sizeof(path), "score/%s.score", m.maze_name);

    if (result < 0 || (size_t)result >= sizeof(path)) {
        fprintf(stderr, "Erreur : le chemin du fichier est trop long\n");
        return 1;
    }
    printf("path : %s\n", path);
    FILE *f = fopen(path, "r+");
    int worst_score = get_worst_score(f);
    if(worst_score > m.score){
        if(f != NULL) fclose(f);
        return 0;
    }

    char player_name[PLAYER_NAME_MAX_LENGTH];
    ask_for_string(PLAYER_NAME_MAX_LENGTH, "Enter your pseudo : ", player_name);

    if (f == NULL) {
        printf("No score found for this maze creating score file...\n");
        create_score_file(path, player_name, m.score);
        return 0;
    } else {
        insert_score_in_file(f, player_name, m.score);
    }
    fclose(f);
    return 0;
}

int get_worst_score(FILE *file){
    if(file == NULL) return -1000;
    int score;
    fseek(file, -sizeof("#10 : 000 : ninechars"), SEEK_END);
    int exists = fscanf(file, "#10 : %d : %*s\n", &score);
    if(!exists) return -1000;
    printf("score : %d\n", score);
    fseek(file, 0, SEEK_SET);
    return score;
}

void create_score_file(char *path, char *player_name, int score) {
    FILE *f = fopen(path, "a");
    if (f == NULL) {
        fprintf(stderr, "Erreur : échec de l'ouverture du fichier\n");
        return;
    }
    fprintf(f, "### SCORE ###\n");
    fprintf(f, "#01 : %3d : %-*s\n", score, PLAYER_NAME_MAX_LENGTH-1, player_name);
    fclose(f);
}

void insert_score_in_file(FILE *file, char *player_name, int score) {
    int scores[11];
    int score_i;
    char name_i[PLAYER_NAME_MAX_LENGTH];
    char names[11][PLAYER_NAME_MAX_LENGTH];
    int i = 0;
    fseek(file, sizeof("### SCORE ###"), SEEK_SET);
    while (i < 10 && fscanf(file, "#%*d : %d : %s\n", &score_i, name_i) == 2){
        scores[i] = score_i;
        memcpy(names[i], name_i, sizeof(char) * PLAYER_NAME_MAX_LENGTH);
        i++;
    }
    scores[i] = score;
    memcpy(names[i], player_name, sizeof(char) * PLAYER_NAME_MAX_LENGTH);

    merge_sort_score(scores, names, 0, i);

    fseek(file, sizeof("### SCORE ###"), SEEK_SET);
    for (int j = 0; j <= i && j < 10; j++) {
        fprintf(file, "#%2d : %3d : %-*s\n", j + 1, scores[j], PLAYER_NAME_MAX_LENGTH-1, names[j]);
    }
}

void merge_score(int *score, char names[11][PLAYER_NAME_MAX_LENGTH], int left, int mid, int right) {
    int i, j, k;
    int left_length = mid - left + 1;
    int right_length = right - mid;

    int left_score[left_length];
    int right_score[right_length];
    char left_names[left_length][PLAYER_NAME_MAX_LENGTH];
    char right_names[right_length][PLAYER_NAME_MAX_LENGTH];

    for (i = 0; i < left_length; i++) {
        left_score[i] = score[left + i];
        memcpy(left_names[i], names[left + i], sizeof(char) * PLAYER_NAME_MAX_LENGTH);
    }
    for (j = 0; j < right_length; j++) {
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
        } else {
            score[k] = right_score[j];
            memcpy(names[k], right_names[j], sizeof(char) * PLAYER_NAME_MAX_LENGTH);
            j++;
        }
        k++;
    }

    while (i < left_length) {
        score[k] = left_score[i];
        memcpy(names[k], left_names[i], sizeof(char) * PLAYER_NAME_MAX_LENGTH);
        i++;
        k++;
    }

    while (j < right_length) {
        score[k] = right_score[j];
        memcpy(names[k], right_names[j], sizeof(char) * PLAYER_NAME_MAX_LENGTH);
        j++;
        k++;
    }
}

void merge_sort_score(int *score, char names[11][PLAYER_NAME_MAX_LENGTH], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        merge_sort_score(score, names, left, mid);
        merge_sort_score(score, names, mid + 1, right);

        merge_score(score, names, left, mid, right);
    }
}