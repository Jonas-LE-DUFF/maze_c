#include "maze.h"

int main() {

    maze *m = malloc(sizeof(maze));

    do {
        show_menu();
    } while (player_interact(m));

    free_maze(m);
    free(m);

    return 0;
}