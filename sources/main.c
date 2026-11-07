#include "maze.h"

int main() {
    srand(time(NULL));

    maze *m = calloc(1, sizeof(maze));

    do {
        show_menu();
    } while (player_interact(m));

    free_maze(m);
    free(m);

    return 0;
}