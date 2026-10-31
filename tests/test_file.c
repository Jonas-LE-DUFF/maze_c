#include "test.h"

MU_TEST(test_create_maze_file) {
    maze *m = malloc(sizeof(maze));
    
    save_maze(m, "mazes/test_config.cfg");

    FILE *f = fopen("mazes/test_config.cfg", "r");
    mu_assert(f != NULL, "create maze should create a maze file in mazes with .cfg extension\n");
    fclose(f);
    remove("mazes/test_config.cfg");
    free(m);
}

MU_TEST(test_load_maze_file) {
    maze *m = malloc(sizeof(maze));
    generate_perfect_maze(m, 9, 7);
    strcpy(m->maze_name, "test_load");
    save_maze(m, "mazes/test_load.cfg");

    maze *loaded_maze = malloc(sizeof(maze));
    strcpy(loaded_maze->maze_name, "test_load");

    file_maze_load(loaded_maze);

    mu_assert_int_eq(9, loaded_maze->height);
    mu_assert_int_eq(7, loaded_maze->width);
    mu_assert(loaded_maze->map != NULL, "when loading map shouldn't be NULL\n");
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            mu_assert_int_eq(m->map[i][j], loaded_maze->map[i][j]);
        }
    }
    mu_assert_string_eq("test_load", loaded_maze->maze_name);
    
    remove("mazes/test_load.cfg");

    free(m);
    free(loaded_maze);
}

MU_TEST(test_score_file_creation) {
    create_score_file("score/test_score.score", "Agathe", 100);

    FILE *f = fopen("score/test_score.score", "r");
    int row;
    int score;
    char name[PLAYER_NAME_MAX_LENGTH];
    
    mu_assert(f != NULL, "handle score should create a new file when it doesn't exist\n");
    mu_assert_int_eq(0, fscanf(f, "### SCORE ###\n"));
    mu_assert_int_eq(3, fscanf(f, "#%d : %d : %s\n", &row, &score, name));
    mu_assert_int_eq(1, row);
    mu_assert_int_eq(100, score);
    mu_assert_string_eq("Agathe", name);
    
    remove("score/test_score.score");
    fclose(f);
}

MU_TEST(test_score_file_adding) {
    create_score_file("score/test_insert.score", "Agathe", 100);

    FILE *f = fopen("score/test_insert.score", "r+");
    mu_assert(f != NULL, "handle score should create a new file when it doesn't exist\n");
    insert_score_in_file(f, "pro", 151);
    insert_score_in_file(f, "noob", 4);
    insert_score_in_file(f, "legend", 200);
    insert_score_in_file(f, "ultra_centre", 50);
    insert_score_in_file(f, "ultra_nul", 0);
    insert_score_in_file(f, "ultra_mauvais", 1);
    insert_score_in_file(f, "bon", 120);
    insert_score_in_file(f, "raph_the_barber", 101);
    insert_score_in_file(f, "ultra_legend", 244);
    insert_score_in_file(f, "genZ", 67);

    int score = 255;
    int new_score = 0;
    char name[PLAYER_NAME_MAX_LENGTH];
    fseek(f, 0, SEEK_SET);
    fscanf(f, "### SCORE ###\n");
    for(int i = 0; i < 10; i++){
        if(fscanf(f, "#%*d : %d : %s\n", &new_score, name) != 2){
            break;
        }
        mu_assert(new_score <= score, "Scores should be sorted in decresending order)");
        score = new_score;
    }
    mu_assert_string_eq("ultra_mauvais", name);
    //shouldn't be ultra_nul as we're keeping only the ten best score

    remove("score/test_insert.score");
    fclose(f);
}

MU_TEST_SUITE(test_file) {
    MU_RUN_TEST(test_create_maze_file);
    MU_RUN_TEST(test_load_maze_file);
    MU_RUN_TEST(test_score_file_creation);
    MU_RUN_TEST(test_score_file_adding);
}

int run_test(void) {
    MU_RUN_SUITE(test_file);

    MU_REPORT();
    return MU_EXIT_CODE;
}

int main() {
    run_test();
    remove("score/test_insert.score");
    remove("score/test_score.score");
}