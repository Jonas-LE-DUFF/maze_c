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
    maze m = {NULL, 9, 9, 100, "test_score"};
    create_score_file("score/test_score.score", "Agathe", 100);

    FILE *f = fopen("score/test_score.score", "r");
    char line[100];
    fread(line, 100, 1, f);
    printf("%s\n", line);

    mu_assert(f != NULL, "handle score should create a new file when it doesn't exist\n");
    
    remove("score/test_score.score");
    fclose(f);
}

MU_TEST_SUITE(test_file) {
    MU_RUN_TEST(test_create_maze_file);
    MU_RUN_TEST(test_load_maze_file);
    MU_RUN_TEST(test_score_file_creation);
}

int run_test(void) {
    MU_RUN_SUITE(test_file);

    MU_REPORT();
    return MU_EXIT_CODE;
}

int main() {
    run_test();
}