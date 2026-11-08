#include "../headers/score_handler.h"

/**
 * @brief This struct contains all the information from one line of a score file
 * this struct allows score lines to be sorted
 */
typedef struct{
    int score;
    char pseudo[PLAYER_NAME_MAX_LENGTH];
} score_row;

/**
 * @brief Helper function to sort score lines
 */
int compare(const void *a, const void *b){
    return (* (score_row*)b).score - (* (score_row*)a).score;
}

int handle_score(maze m) {
    char path[MAX_FILE_PATH_SIZE];
    int result = snprintf(path, sizeof(path), "scores/%s.score", m.maze_name);

    if (result < 0 || (size_t)result >= sizeof(path)) {
        fprintf(stderr, "Erreur : le chemin du fichier est trop long\n");
        return 1;
    }
    
    FILE *f = fopen(path, "r+");
    int worst_score = get_worst_score(f);
    if(worst_score > m.score){
        // not in top 10 scores... nothing to do
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

/**
 * @brief Get the worst score of the file by looking at the 11th line
 * 
 * @param file score file
 * @return int the 10th's score or -1000 if it doesn't exist yet
 */
int get_worst_score(FILE *file){
    if(file == NULL) return -1000; //no score file returning vert low score
    int score;
    //going at the begining of the last line
    fseek(file, -sizeof("#10 : 000 : ninechars"), SEEK_END);
    int exists = fscanf(file, "#10 : %d : %*s\n", &score);
    if(!exists) return -1000; // no 11th line returning very low score
    fseek(file, 0, SEEK_SET);
    return score;
}

/**
 * @brief Create a new score file with the player's score in it
 * 
 * @param path the path to the new file
 * @param player_name the player's pseudo
 * @param score the score put in the file
 */
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

/**
 * @brief adds a new score to the score file.
 * keeps the score sorted and keeps only the 10 best scores 
 * 
 * @param file the score file
 * @param player_name the player's pseudo
 * @param score the score to add to the score file
 */
void insert_score_in_file(FILE *file, char *player_name, int score) {
    score_row score_row_i;
    score_row score_rows[11];
    int i = 0;

    //fetching the scores from the file
    fseek(file, sizeof("### SCORE ###"), SEEK_SET);
    while (i < 10 && fscanf(file, "#%*d : %d : %s\n", &score_row_i.score, score_row_i.pseudo) == 2){
        score_rows[i] = score_row_i;
        i++;
    }

    // adding the new scores to the file's score
    score_row_i.score = score;
    memcpy(score_row_i.pseudo, player_name, sizeof(char) * PLAYER_NAME_MAX_LENGTH);
    score_rows[i] = score_row_i;

    // sorting the scores
    qsort(score_rows, i+1, sizeof(score_row), compare);

    // printing the new best scores to the file
    fseek(file, sizeof("### SCORE ###"), SEEK_SET);
    for (int j = 0; j <= i && j < 10; j++) {
        fprintf(file, "#%2d : %3d : %-*s\n", j + 1, score_rows[j].score, PLAYER_NAME_MAX_LENGTH-1, score_rows[j].pseudo);
    }
}
