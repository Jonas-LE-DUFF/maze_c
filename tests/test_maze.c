#include "test.h"

#define MAZE_SIZE 5

maze m;

void test_maze_fill_setup(void) {
    tile map[MAZE_SIZE][MAZE_SIZE] = {
        {WALL, PLAYER, WALL, WALL, WALL},  {WALL, EMPTY, WALL, EMPTY, WALL},
        {WALL, EMPTY, EMPTY, EMPTY, WALL}, {WALL, WALL, WALL, EMPTY, WALL},
        {WALL, WALL, WALL, EXIT, WALL},
    };
    init_maze(&m, MAZE_SIZE, MAZE_SIZE);

    for (int i = 0; i < MAZE_SIZE; i++) {
        memcpy(m.map[i], map[i], MAZE_SIZE * sizeof(tile));
    }
}

void test_maze_gen_setup(void) {
    init_maze(&m, 5, 9);
}

void test_maze_fill_items_feat(void) {
    tile map[MAZE_SIZE][MAZE_SIZE] = {
        {WALL, PLAYER, WALL, WALL, WALL},      {WALL, EMPTY, WALL, KEY, WALL},
        {WALL, TREASURE, TRAP, EMPTY, WALL},   {WALL, WALL, WALL, EMPTY, WALL},
        {WALL, WALL, WALL, LOCKED_DOOR, WALL},
    };
    init_maze(&m, MAZE_SIZE, MAZE_SIZE);
    for (int i = 0; i < MAZE_SIZE; i++) {
        memcpy(m.map[i], map[i], MAZE_SIZE * sizeof(tile));
    }
    m.score = SCORE_START;
}

void teardown(void) {
    free_maze(&m);
}

MU_TEST(init_test) {
    free_maze(&m);
    init_maze(&m, 5, 9);
    mu_assert(m.height == 5, "init_maze should define maze height in the maze struct");
    mu_assert(m.width == 9, "init_maze should define maze width  in the maze struct");
    mu_assert(m.score == SCORE_START, "init_maze should define the score to SCORE_START");
    mu_assert(m.map[3][0] == WALL, "init_maze should put walls on all the outside tiles");
    mu_assert(m.map[2][2] == WALL, "init_maze should define all inside walls");
    mu_assert(m.map[1][1] == EMPTY, "init_maze should define the rooms needed for the algorithm");
}

MU_TEST(generate_perfect_maze_test1) {
    m = (maze){0};
    mu_assert(generate_perfect_maze(&m, 5, 8) == 0,
              "generate_perfect_maze should not do anything if the given height or width is even");
    free_maze(&m);
    generate_perfect_maze(&m, 5, 9);
    mu_assert(m.map[0][1] == PLAYER, "generate_perfect_maze should place the player");
    mu_assert(m.map[4][7] == LOCKED_DOOR, "generate_perfect_maze should place the door");
}

MU_TEST(handle_move_test) {
    handle_move(&m, DOWN);
    mu_assert(m.map[1][1] == PLAYER, "handle move should move the player");
    mu_assert(m.map[0][1] == EMPTY, "handle move should clear the tile the player was at");
    handle_move(&m, LEFT);
    mu_assert(m.map[1][1] == PLAYER,
              "handle move shouldn't move the player when moving into a wall");
    mu_assert(m.map[0][1] == EMPTY,
              "handle move shouldn't clear the tile the player was at when moving into a wall");
    handle_move(&m, DOWN);
    handle_move(&m, RIGHT);
    handle_move(&m, RIGHT);
    mu_check(handle_move(&m, DOWN) == 0);
    mu_assert_int_eq(1, handle_move(&m, DOWN));
}

MU_TEST(test_score) {
    handle_move(&m, DOWN);
    mu_assert(m.score == SCORE_START - SCORE_LOSS, "score should go down after each move");
    handle_move(&m, DOWN);
    mu_assert(m.score == SCORE_START - SCORE_LOSS * 2 + TREASURE_BONUS,
              "score should increase when player moves into treasure");
    handle_move(&m, RIGHT);
    mu_assert(m.score == SCORE_START - SCORE_LOSS * 3 + TREASURE_BONUS - TRAP_MALUS,
              "score should decrease when player moves into trap");
    handle_move(&m, RIGHT);
    handle_move(&m, DOWN);
    handle_move(&m, DOWN);
    mu_assert(m.map[4][3] == LOCKED_DOOR, "door should remain locked until player takes the key");
    mu_assert(m.map[3][3] == PLAYER,
              "locked door should behave like a wall when player tries to walk in it");

    handle_move(&m, UP);
    handle_move(&m, UP);
    mu_assert(m.map[4][3] == EXIT, "door should unlock when player takes the key");
    handle_move(&m, DOWN);
    handle_move(&m, DOWN);
    mu_assert_int_eq(1, handle_move(&m, DOWN));
}

MU_TEST_SUITE(test_maze_gen) {
    MU_SUITE_CONFIGURE(NULL, &teardown);
    MU_RUN_TEST(init_test);
    MU_RUN_TEST(generate_perfect_maze_test1);
}

MU_TEST_SUITE(test_maze_play) {
    MU_SUITE_CONFIGURE(&test_maze_fill_setup, &teardown);
    MU_RUN_TEST(handle_move_test);
}

MU_TEST_SUITE(test_maze_points) {
    MU_SUITE_CONFIGURE(&test_maze_fill_items_feat, &teardown);
    MU_RUN_TEST(test_score);
}

int run_test(void) {
    MU_RUN_SUITE(test_maze_gen);
    MU_RUN_SUITE(test_maze_play);
    MU_RUN_SUITE(test_maze_points);

    MU_REPORT();
    return MU_EXIT_CODE;
}

int main() {
    run_test();
}